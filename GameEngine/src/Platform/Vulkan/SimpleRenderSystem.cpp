#include "pch.h"
#include "SimpleRenderSystem.h"
#include "Device.h"
#include "GraphicsPipeline.h"
#include "Descriptors.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	SimpleRenderSystem::SimpleRenderSystem(VkRenderPass renderpass, VkDescriptorSetLayout setLayout)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		CreatePipelineLayout(setLayout);
		CreatePipeline(renderpass);

	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
	
		vkDestroyPipelineLayout(*m_Device, m_PipelineLayout, nullptr);

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

	
		if (vkCreatePipelineLayout(*m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
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
		m_GraphicsPipeline = MakeScope<GraphicsPipeline>("assets/shaders/vert.spv", "assets/shaders/frag.spv", pipelineConfigInfo);
	}
}


