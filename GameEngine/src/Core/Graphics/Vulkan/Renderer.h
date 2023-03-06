#pragma once

#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

class Renderer
{
public:

	Renderer(class Device* device, class WinWindow* window);
	~Renderer();

public:

	VkRenderPass GetSwapChainRenderPass() const;

	VkExtent2D GetSwapChainExtents() const;

	bool IsFrameInProgress() const { return m_IsFrameStarted; };

	VkCommandBuffer GetCurrentCommandBuffer() const;

	uint32_t GetSwapChainCurrentFrame() const;

	VkCommandBuffer BeginFrame();
	void EndFrame();
	void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:

	void CeateCommandBuffers();

	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void FreeCommandBuffers();

	void RecreateSwapChain();

private:

	std::unique_ptr<class SwapChain> m_SwapChain;

	std::vector<VkCommandBuffer> m_CommandBuffers;

	bool m_IsFrameStarted = false;

	uint32_t m_CurrentFrameIndex = 0;

	class WinWindow* m_Window;
	class Device* m_Device;
};