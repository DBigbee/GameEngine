#include "pch.h"
#include "VulkanContext.h"
#include "Core/WinWindow.h"
#include "Core/Application.h"
#include "Mesh/Mesh.h"
#include "Vulkan/Device.h"
#include "Vulkan/GraphicsPipeline.h"
#include "Vulkan/SwapChain.h"


VulkanContext::VulkanContext(WinWindow* window)
	:GraphicsContext(window), m_Window(window)
{	
	m_Device = std::make_unique<Device>(window);

	CreatePipelineLayout();
	RecreateSwapChain();
	CeateCommandBuffers();

	CreateVertexBuffer();
}

VulkanContext::~VulkanContext()
{
	vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);
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

	m_SwapChain = std::make_unique<SwapChain>(m_Device.get(), extent);
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

	m_Triangle->Draw(commandBuffer);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffers!");
	}
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
		{{0.0f, -.5f, 0.0f},{1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f},{0.0f, 1.0f, 0.0f}},
		{{-.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f}}
	};
	m_Triangle = std::make_unique<Mesh>(m_Device.get(), vertices);
}

void VulkanContext::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(m_Device->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout!");
	}

}

void VulkanContext::CreatePipeline()
{
	auto extents = m_SwapChain->GetExtents();
	auto pipelineConfig = GraphicsPipeline::CreateDefaultPipelineConfigInfo(extents.width, extents.height);
	pipelineConfig.m_RenderPass = m_SwapChain->GetRenderPass();
	pipelineConfig.m_PipelineLayout = m_PipelineLayout;
	m_GraphicsPipeline = std::make_unique<GraphicsPipeline>(m_Device.get(), "assets/shaders/vert.spv", "assets/shaders/frag.spv", pipelineConfig);

}
