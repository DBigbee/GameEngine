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
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(FSimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ setLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	
		if (vkCreatePipelineLayout(*m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			GE_CORE_ASSERT(false, "Failed to create pipeline layout!");
		}
	}

	void SimpleRenderSystem::CreatePipeline(VkRenderPass renderpass)
	{
		GE_CORE_ASSERT(m_PipelineLayout != nullptr , "Cannot create pipeline before pipeline layout!");

		PipelineConfigInfo pipelineConfigInfo{};
		GraphicsPipeline::CreateDefaultPipelineConfigInfo(pipelineConfigInfo);
		pipelineConfigInfo.m_RenderPass = renderpass;
		pipelineConfigInfo.m_PipelineLayout = m_PipelineLayout;
		m_GraphicsPipeline = MakeScope<GraphicsPipeline>("assets/shaders/shader.glsl", pipelineConfigInfo);
	}
}


