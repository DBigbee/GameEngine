#pragma once

#include <vulkan/vulkan.h>
#include "QueueFamilyIndices.h"

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR m_Capabilities{};
	std::vector<VkSurfaceFormatKHR> m_Formats;
	std::vector<VkPresentModeKHR> m_PresentModes;
};


class Device
{
public:
#ifdef GE_RELEASE
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	Device(class WinWindow* window);

	~Device();

	VkSurfaceKHR GetSurface() { return m_Surface; }

	VkDevice GetDevice() { return m_Device; }

	VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }

	VkQueue GetPresentQueue() { return m_PresentQueue; }

	VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }

	VkCommandPool GetCommandPool() { return m_CommandPool; }

private:
	void CreateVKInstance();

	void CheckVKExtensions();

	void SetupDebugMessenger();

	void SelectPhysicalDevice();

	void CreateLogicalDevice();



public:
	bool IsDeviceSuitable(VkPhysicalDevice device) const;

	int RateDeviceSuitability(VkPhysicalDevice device);

	bool CheckValidationLayerSupport() const;

	std::vector<const char*> GetRequiredExtensions() const;

	QueueFamilityIndices FindQueueFamilies(VkPhysicalDevice device) const;

	uint32_t FindMemoryType(uint32_t FilterType, VkMemoryPropertyFlags properties);


	bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

	void AllocateMemory(VkDeviceSize size, uint32_t filterType, VkDeviceMemory memory, VkMemoryPropertyFlags properties);

	VkPhysicalDeviceProperties GetProperties() const { return m_Properties; }

private:
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void CreateWindowSurface();

	void CreateCommandPool();

private:

	VkQueue m_GraphicsQueue = VK_NULL_HANDLE;

	VkQueue m_PresentQueue = VK_NULL_HANDLE;

	VkInstance m_VKInstance = VK_NULL_HANDLE;

	VkDebugUtilsMessengerEXT m_VKDebugMessenger = VK_NULL_HANDLE;

	VkDevice m_Device = VK_NULL_HANDLE;

	VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

	VkCommandPool m_CommandPool = VK_NULL_HANDLE;

	VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

	class WinWindow* m_Window = nullptr;

	VkPhysicalDeviceProperties m_Properties;

	const std::vector<const char*> G_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

	const std::vector<const char*> G_DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME  };
};