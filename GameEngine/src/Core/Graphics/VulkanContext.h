#pragma once

#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

#include "GraphicsContext.h"


class VulkanContext : public GraphicsContext
{
public:

	VulkanContext(WinWindow* window);

	~VulkanContext();

	void WaitIdle() override;

private:

	void CeateCommandBuffers();

	void RecreateSwapChain();

	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void DrawFrame() override;

	void CreatePipelineLayout();

	void CreatePipeline();

	void CreateVertexBuffer();

	void CreateUniformBuffers();

	void UpdateUniformBuffer(uint32_t currentImage);

	void CreateDescriptorPool();

	void CreateDescriptorSet();

private:

	std::unique_ptr<class Device> m_Device;

	std::unique_ptr<class Mesh> m_Triangle;

	std::unique_ptr<class GraphicsPipeline> m_GraphicsPipeline;

	std::unique_ptr<class SwapChain> m_SwapChain;

	VkPipelineLayout m_PipelineLayout;

	VkDescriptorPool m_DescriptorPool;

	std::vector<VkDescriptorSet> m_DescriptorSets;

	std::vector<VkCommandBuffer> m_CommandBuffers;

	std::unique_ptr<class DescriptorSetLayout> m_DescriptorSetLayout;

	std::vector< std::unique_ptr<class UniformBuffer>> m_UniformBuffers;

	class WinWindow* m_Window;

};