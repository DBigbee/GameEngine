#pragma once

#include <vulkan/vulkan.h>

namespace GE
{
	struct PipelineConfigInfo
	{
		VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo m_RasterizerInfo{};
		VkPipelineColorBlendAttachmentState m_ColorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo m_ColorBlendInfo{};
		VkPipelineMultisampleStateCreateInfo m_MultisamplingInfo{};
		VkPipelineDepthStencilStateCreateInfo m_DepthStencilInfo{};
		std::vector<VkDynamicState> m_DynamicStatesEnabled;
		VkPipelineDynamicStateCreateInfo m_DynamicStateInfo{};

		VkPipelineLayout m_PipelineLayout = nullptr;
		VkRenderPass m_RenderPass = nullptr;
		uint32_t m_Subpass = 0;
	};

	class GraphicsPipeline
	{
	public:

		GraphicsPipeline(const std::string& shaderFilePath, const PipelineConfigInfo& configInfo);

		~GraphicsPipeline();

		void Bind(VkCommandBuffer commandBuffer);

		static void CreateDefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:

		void CreateGraphicsPipeline(const std::string& shaderFilePath, const PipelineConfigInfo& configInfo);

		VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

		Ref<class Shader> m_Shader;

		Ref<class Device> m_Device;
	};
}