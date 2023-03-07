#pragma once

#include "Device.h"


class SwapChain
{
public:
	static const int MAX_FRAMES_IN_FLIGHT = 2;

	SwapChain(class Device* device, VkExtent2D extent);
	SwapChain(class Device* device, VkExtent2D extent, std::shared_ptr<class SwapChain> previous);

	~SwapChain();

	VkExtent2D GetExtents() const { return m_Extent; }

	VkRenderPass GetRenderPass() const { return m_RenderPass; }

	std::vector<VkFramebuffer> GetFrameBuffers() const { return m_Framebuffers; }

	std::vector < VkSemaphore> GetAvailableImages() const { return m_ImageAvailableSemaphores; }

	std::vector < VkSemaphore> GetFinishedImages() const { return m_RenderFinishedSemaphores; }

	VkSwapchainKHR GetSwapChain() const { return m_SwapChain; }

	VkResult AquireNextImage(uint32_t* imageIndex);

	void ResetFence();

	VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

	uint32_t GetCurrentFrame() const { return m_CurrentFrame; }

	size_t GetImageCount() const { return m_Images.size(); }

protected:
	void Init();

	void CreateSwapChain();

	void CreateImageViews();

	void CreateRenderPass();

	void CreateFramebuffers();

	void CreateSyncObjects();

	void CleanUp();

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;

	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

private:


	VkSwapchainKHR m_SwapChain;

	std::vector<VkImage> m_Images;

	VkFormat m_ImageFormat;

	VkExtent2D m_Extent;

	VkRenderPass m_RenderPass;

	std::vector<VkImageView> m_ImageViews;

	std::vector<VkFramebuffer> m_Framebuffers;

	std::vector < VkSemaphore> m_ImageAvailableSemaphores;

	std::vector < VkSemaphore> m_RenderFinishedSemaphores;

	std::vector < VkFence> m_InFlightFences;

	uint32_t m_CurrentFrame = 0;

	class Device* m_Device;

	std::shared_ptr<class SwapChain> m_OldSwapChain = nullptr;
};