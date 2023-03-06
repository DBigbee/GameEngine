#pragma once

#include <vulkan/vulkan.h>

class SimpleRenderSystem
{
public:
	const int MAX_FRAMES_IN_FLIGHT = 2;

	SimpleRenderSystem(class Device* device, VkRenderPass renderpass);

	~SimpleRenderSystem();

	void RenderObjects(VkCommandBuffer commandBuffer, uint32_t currentFrame, uint32_t width, uint32_t height, const std::vector<class Mesh*>& objects);

private:
	void CreatePipelineLayout();

	void CreatePipeline(VkRenderPass renderpass);

	void CreateUniformBuffers();

	void UpdateUniformBuffer(uint32_t currentImage, uint32_t width, uint32_t height);

	void CreateDescriptorPool();

	void CreateDescriptorSet();

private:

	std::unique_ptr<class GraphicsPipeline> m_GraphicsPipeline;

	VkDescriptorPool m_DescriptorPool;

	std::vector<VkDescriptorSet> m_DescriptorSets;

	std::unique_ptr<class DescriptorSetLayout> m_DescriptorSetLayout;

	std::vector< std::unique_ptr<class UniformBuffer>> m_UniformBuffers;

	class Device* m_Device = nullptr;

	VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
};