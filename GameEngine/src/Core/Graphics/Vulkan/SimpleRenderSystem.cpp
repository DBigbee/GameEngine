#include "pch.h"
#include "SimpleRenderSystem.h"
#include "Device.h"
#include "GraphicsPipeline.h"
#include "Descriptors.h"
#include "Core/Graphics/Mesh/Mesh.h"

SimpleRenderSystem::SimpleRenderSystem(Device* device, VkRenderPass renderpass, VkDescriptorSetLayout setLayout)
	:m_Device(device)
{
	CreatePipelineLayout(setLayout);
	CreatePipeline(renderpass);

}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);

}

void SimpleRenderSystem::RenderObjects(VkCommandBuffer commandBuffer,  const std::vector<class Mesh*>& objects)
{
	m_GraphicsPipeline->Bind(commandBuffer);

	for (auto object : objects)
	{
		object->Bind(commandBuffer);
		object->Draw(commandBuffer);
	}
}

void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout setLayout)
{
	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ setLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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




