#include "pch.h"
#include "SimpleRenderSystem.h"
#include "Device.h"
#include "GraphicsPipeline.h"
#include "DescriptorSetLayout.h"
#include "Core/Graphics/Mesh/UniformBuffer.h"
#include "Core/Graphics/Mesh/Mesh.h"

SimpleRenderSystem::SimpleRenderSystem(Device* device, VkRenderPass renderpass)
	:m_Device(device)
{
	m_DescriptorSetLayout = std::make_unique<DescriptorSetLayout>(m_Device, 0);
	CreatePipelineLayout();
	CreatePipeline(renderpass);
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSet();

}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);
	vkDestroyDescriptorPool(m_Device->GetDevice(), m_DescriptorPool, nullptr);

}

void SimpleRenderSystem::RenderObjects(VkCommandBuffer commandBuffer, uint32_t currentFrame, uint32_t width, uint32_t height, const std::vector<class Mesh*>& objects)
{
	UpdateUniformBuffer(currentFrame, width, height);

	m_GraphicsPipeline->Bind(commandBuffer);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[currentFrame], 0, nullptr);

	for (auto object : objects)
	{
		object->Draw(commandBuffer);
	}
}

void SimpleRenderSystem::CreatePipelineLayout()
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

void SimpleRenderSystem::CreatePipeline(VkRenderPass renderpass)
{
	assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

	PipelineConfigInfo pipelineConfigInfo{};
	GraphicsPipeline::CreateDefaultPipelineConfigInfo(pipelineConfigInfo);
	pipelineConfigInfo.m_RenderPass = renderpass;
	pipelineConfigInfo.m_PipelineLayout = m_PipelineLayout;
	m_GraphicsPipeline = std::make_unique<GraphicsPipeline>(m_Device, "assets/shaders/vert.spv", "assets/shaders/frag.spv", pipelineConfigInfo);
}

void SimpleRenderSystem::CreateUniformBuffers()
{
	m_UniformBuffers.resize((size_t)MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		m_UniformBuffers[i] = std::make_unique<UniformBuffer>(m_Device, static_cast<uint32_t>(sizeof(FUniformBufferObject)));
	}
}

void SimpleRenderSystem::UpdateUniformBuffer(uint32_t currentImage, uint32_t width, uint32_t height)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	FUniformBufferObject ubo{};
	ubo.m_Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.m_View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.m_Projection = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);

	//invert for vulkan
	ubo.m_Projection[1][1] *= -1;

	m_UniformBuffers[currentImage]->Update(sizeof(FUniformBufferObject), &ubo);
}

void SimpleRenderSystem::CreateDescriptorPool()
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

void SimpleRenderSystem::CreateDescriptorSet()
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
