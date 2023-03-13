#include "pch.h"
#include "GraphicsPipeline.h"
#include "Device.h"
#include "Renderer/Vertex.h"
#include "Shader.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{

	GraphicsPipeline::GraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();
		CreateGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		vkDestroyPipeline(*m_Device, m_GraphicsPipeline, nullptr);
	}

	void GraphicsPipeline::CreateDefaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
	{
		configInfo.m_InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.m_InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.m_InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.m_RasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.m_RasterizerInfo.depthClampEnable = VK_FALSE;
		configInfo.m_RasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.m_RasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.m_RasterizerInfo.lineWidth = 1.0f;
		configInfo.m_RasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		configInfo.m_RasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		configInfo.m_RasterizerInfo.depthBiasEnable = VK_FALSE;
		configInfo.m_RasterizerInfo.depthBiasConstantFactor = 0.0f; //optional
		configInfo.m_RasterizerInfo.depthBiasClamp = 0.0f; //optional
		configInfo.m_RasterizerInfo.depthBiasSlopeFactor = 0.0f; //optional

		configInfo.m_MultisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.m_MultisamplingInfo.sampleShadingEnable = VK_FALSE;
		configInfo.m_MultisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.m_MultisamplingInfo.minSampleShading = 1.0f; //optional
		configInfo.m_MultisamplingInfo.pSampleMask = nullptr; //optional
		configInfo.m_MultisamplingInfo.alphaToCoverageEnable = VK_FALSE; //optional
		configInfo.m_MultisamplingInfo.alphaToOneEnable = VK_FALSE; //optional


		configInfo.m_ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.m_ColorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.m_ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.m_ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.m_ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo.m_ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.m_ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.m_ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		configInfo.m_ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.m_ColorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.m_ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		configInfo.m_ColorBlendInfo.attachmentCount = 1;
		configInfo.m_ColorBlendInfo.pAttachments = &configInfo.m_ColorBlendAttachment;
		configInfo.m_ColorBlendInfo.blendConstants[0] = 0.0f;
		configInfo.m_ColorBlendInfo.blendConstants[1] = 0.0f;
		configInfo.m_ColorBlendInfo.blendConstants[2] = 0.0f;
		configInfo.m_ColorBlendInfo.blendConstants[3] = 0.0f;

		configInfo.m_DynamicStatesEnabled = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		configInfo.m_DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.m_DynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.m_DynamicStatesEnabled.size());
		configInfo.m_DynamicStateInfo.pDynamicStates = configInfo.m_DynamicStatesEnabled.data();

		configInfo.m_DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.m_DepthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.m_DepthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.m_DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.m_DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.m_DepthStencilInfo.minDepthBounds = 0.0f;
		configInfo.m_DepthStencilInfo.maxDepthBounds = 1.0f;
		configInfo.m_DepthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.m_DepthStencilInfo.front = {};
		configInfo.m_DepthStencilInfo.back = {};
	}

	void GraphicsPipeline::Bind(VkCommandBuffer commandBuffer)
	{
		if (m_GraphicsPipeline == nullptr) return;

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

	}

	void GraphicsPipeline::CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo)
	{
		assert(configInfo.m_PipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipepline : no pipelinelayout provided in configInfo");
		assert(configInfo.m_RenderPass != VK_NULL_HANDLE && "Cannot create graphics pipepline : no renderPass provided in configInfo");

		m_Shader = MakeScope<Shader>(vertFilePath, fragFilePath);

		VkPipelineShaderStageCreateInfo shaderStages[2] = {};
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = m_Shader->GetShaderModules()[0];
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = m_Shader->GetShaderModules()[1];
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescription = Vertex::GetAttributeDscription();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; //optional

		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data(); //optional




		VkPipelineViewportStateCreateInfo viewportStateInfo{};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount = 1;
		//viewportInfo.pViewports = &configInfo.m_Viewport;
		viewportStateInfo.scissorCount = 1;
		//viewportInfo.pScissors = &configInfo.m_Scissor;


		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &configInfo.m_InputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportStateInfo;
		pipelineInfo.pRasterizationState = &configInfo.m_RasterizerInfo;
		pipelineInfo.pMultisampleState = &configInfo.m_MultisamplingInfo;
		pipelineInfo.pDepthStencilState = &configInfo.m_DepthStencilInfo;
		pipelineInfo.pColorBlendState = &configInfo.m_ColorBlendInfo;
		pipelineInfo.pDynamicState = &configInfo.m_DynamicStateInfo;
		pipelineInfo.layout = configInfo.m_PipelineLayout;
		pipelineInfo.renderPass = configInfo.m_RenderPass;
		pipelineInfo.subpass = configInfo.m_Subpass;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

	
		if (vkCreateGraphicsPipelines(*m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline!");
		}


	}
}