#pragma once

#include <vulkan/vulkan.h>

class SimpleRenderSystem
{
public:
	const int MAX_FRAMES_IN_FLIGHT = 2;

	SimpleRenderSystem(class Device* device, VkRenderPass renderpass, VkDescriptorSetLayout setLayout);

	~SimpleRenderSystem();

	void RenderObjects(VkCommandBuffer commandBuffer, const std::vector<class Model*>& objects);

	class GraphicsPipeline* GetPipeline() const { return m_GraphicsPipeline.get(); }

	VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

private:
	void CreatePipelineLayout(VkDescriptorSetLayout setLayout);

	void CreatePipeline(VkRenderPass renderpass);


private:

	std::unique_ptr<class GraphicsPipeline> m_GraphicsPipeline;

	class Device* m_Device = nullptr;

	VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
};