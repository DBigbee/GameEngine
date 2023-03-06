#pragma once

#include <vulkan/vulkan.h>

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

	GraphicsPipeline(class Device* device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

	~GraphicsPipeline();

	void Bind(VkCommandBuffer commandBuffer);

	static void CreateDefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

private:

	void CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

	class Device* m_Device = nullptr;

	VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;


	std::unique_ptr<class Shader> m_Shader;
};