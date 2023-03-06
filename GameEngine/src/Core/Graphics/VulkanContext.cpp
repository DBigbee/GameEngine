#include "pch.h"
#include "VulkanContext.h"
#include "Core/WinWindow.h"
#include "Core/Application.h"
#include "Mesh/Mesh.h"
#include "Vulkan/Device.h"
#include "Vulkan/GraphicsPipeline.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/DescriptorSetLayout.h"
#include "Mesh/UniformBuffer.h"

void sierpinski(std::vector<Vertex>& vertices, int depth, glm::vec3 left, glm::vec3 right, glm::vec3 top)
{
	if (depth <= 0)
	{
		vertices.push_back({ top,  {0.0f, 0.0f, 1.0f} });
		vertices.push_back({ right, {0.0f, 0.0f, 1.0f} });
		vertices.push_back({ left, {0.0f, 0.0f, 1.0f} });
	}
	else
	{
		auto leftTop = 0.5f * (left + top);
		auto rightTop = 0.5f * (right + top);
		auto leftRight = 0.5f * (left + right);
		sierpinski(vertices, depth - 1, left, leftRight, leftTop);
		sierpinski(vertices, depth - 1, leftRight, right, rightTop);
		sierpinski(vertices, depth - 1, leftTop, rightTop, top);
	}
}

VulkanContext::VulkanContext(WinWindow* window)
	:GraphicsContext(window), m_Window(window)
{	
	m_Device = std::make_unique<Device>(window);
	m_DescriptorSetLayout = std::make_unique<DescriptorSetLayout>(m_Device.get(), 0);

	CreatePipelineLayout();
	RecreateSwapChain();
	CeateCommandBuffers();

	CreateVertexBuffer();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSet();
}

VulkanContext::~VulkanContext()
{
	vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);

	vkDestroyDescriptorPool(m_Device->GetDevice(), m_DescriptorPool, nullptr);
}

void VulkanContext::WaitIdle()
{
	vkDeviceWaitIdle(m_Device->GetDevice());
}

void VulkanContext::CeateCommandBuffers()
{
	m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_Device->GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

	if (vkAllocateCommandBuffers(m_Device->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command buffers!");
	}
}

void VulkanContext::RecreateSwapChain()
{

	auto extent = m_Window->GetExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = m_Window->GetExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_Device->GetDevice());

	if (m_SwapChain == nullptr)
	{
		m_SwapChain = std::make_unique<SwapChain>(m_Device.get(), extent);
	}
	else
	{
		m_SwapChain = std::make_unique<SwapChain>(m_Device.get(), extent, std::move(m_SwapChain));
		if (m_SwapChain->GetImageCount() != m_CommandBuffers.size())
		{
			FreeCommandBuffers();
			CeateCommandBuffers();
		}
	}

	//check if renderpass compatable before creating new pipeline
	CreatePipeline();
}

void VulkanContext::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording commandbuffer!");
	}

	VkRenderPassBeginInfo renderpassInfo{};
	renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderpassInfo.renderPass = m_SwapChain->GetRenderPass();
	renderpassInfo.framebuffer = m_SwapChain->GetFrameBuffers()[imageIndex];

	renderpassInfo.renderArea.offset = { 0 ,0 };
	renderpassInfo.renderArea.extent = m_SwapChain->GetExtents();

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderpassInfo.clearValueCount = 1;
	renderpassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);

	m_GraphicsPipeline->Bind(commandBuffer);

	auto extents = m_SwapChain->GetExtents();
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extents.width;
	viewport.height = (float)extents.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0 ,0 };
	scissor.extent = m_SwapChain->GetExtents();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	auto currentFrame = m_SwapChain->GetCurrentFrame();
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[currentFrame], 0, nullptr);
	m_Triangle->Draw(commandBuffer);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffers!");
	}
}

void VulkanContext::FreeCommandBuffers()
{
	vkFreeCommandBuffers(m_Device->GetDevice(), m_Device->GetCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	m_CommandBuffers.clear();
}

void VulkanContext::DrawFrame()
{
	uint32_t imageIndex;
	VkResult result = m_SwapChain->AquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed tp acquire swap chain image!");
	}

	
	auto currentFrame = m_SwapChain->GetCurrentFrame();

	UpdateUniformBuffer(currentFrame);

	m_SwapChain->ResetFence();
	vkResetCommandBuffer(m_CommandBuffers[currentFrame], 0);
	RecordCommandBuffer(m_CommandBuffers[currentFrame], imageIndex);

	result = m_SwapChain->SubmitCommandBuffers(m_CommandBuffers.data(), &imageIndex);	
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window->WasWindowResized())
	{
		m_Window->ResetWindowResizedFlag();
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

}

void VulkanContext::CreateVertexBuffer()
{
	std::vector<Vertex> vertices =
	{
		{{-0.5f, -.5f, 0.0f},{1.0f, 0.0f, 0.0f}},
		{{0.5f, -.5f, 0.0f},{1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f},{0.0f, 1.0f, 0.0f}},
		{{-.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f}}
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 2, 2, 3, 0
	};
	
	m_Triangle = std::make_unique<Mesh>(m_Device.get(), vertices, indices);
}

void VulkanContext::CreateUniformBuffers()
{
	m_UniformBuffers.resize((size_t)MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		m_UniformBuffers[i] = std::make_unique<UniformBuffer>(m_Device.get(), static_cast<uint32_t>(sizeof(FUniformBufferObject)));
	}
}

void VulkanContext::UpdateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	FUniformBufferObject ubo{};
	ubo.m_Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.m_View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	if(m_SwapChain)
		ubo.m_Projection = glm::perspective(glm::radians(45.0f), m_SwapChain->GetExtents().width / (float)m_SwapChain->GetExtents().height, 0.1f, 10.0f);

	//invert for vulkan
	ubo.m_Projection[1][1] *= -1;

	m_UniformBuffers[currentImage]->Update(sizeof(FUniformBufferObject), &ubo);
}

void VulkanContext::CreateDescriptorPool()
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(m_Device->GetDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor pool!");
	}
}

void VulkanContext::CreateDescriptorSet()
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout->GetSetLayout());

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_DescriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(m_Device->GetDevice(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor sets!");
	}
	
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_UniformBuffers[i]->GetBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(FUniformBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(m_Device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}

void VulkanContext::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout->GetSetLayout();
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(m_Device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout!");
	}

}

void VulkanContext::CreatePipeline()
{
	assert(m_SwapChain != nullptr && "Cannot create pipeline before swap chain!");
	assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

	auto extents = m_SwapChain->GetExtents();
	auto pipelineConfig = GraphicsPipeline::CreateDefaultPipelineConfigInfo(extents.width, extents.height);
	pipelineConfig.m_RenderPass = m_SwapChain->GetRenderPass();
	pipelineConfig.m_PipelineLayout = m_PipelineLayout;
	m_GraphicsPipeline = std::make_unique<GraphicsPipeline>(m_Device.get(), "assets/shaders/vert.spv", "assets/shaders/frag.spv", pipelineConfig);

}
