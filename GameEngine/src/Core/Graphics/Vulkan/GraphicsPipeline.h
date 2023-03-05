#pragma once

#include <vulkan/vulkan.h>

struct PipelineConfigInfo
{
	VkViewport m_Viewport;
	VkRect2D m_Scissor;

	VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyInfo{};
	VkPipelineRasterizationStateCreateInfo m_RasterizerInfo{};
	VkPipelineColorBlendAttachmentState m_ColorBlendAttachment{};
	VkPipelineColorBlendStateCreateInfo m_ColorBlendInfo{};
	VkPipelineMultisampleStateCreateInfo m_MultisamplingInfo{};
	VkPipelineDepthStencilStateCreateInfo m_DepthStencilInfo{};

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

	static PipelineConfigInfo CreateDefaultPipelineConfigInfo(uint32_t width, uint32_t height);

private:

	static std::string ReadFile(const std::string& filename);

	void CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

	VkShaderModule CreateShaderModule(const std::string& code);

	class Device* m_Device = nullptr;

	VkPipeline m_GraphicsPipeline = VK_NULL_HANDLE;

	VkShaderModule m_VertShaderModule = VK_NULL_HANDLE;

	VkShaderModule m_FragShaderModule = VK_NULL_HANDLE;
};