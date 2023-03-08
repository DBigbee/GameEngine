#pragma once

#include <vulkan/vulkan.h>

class Image
{
public:

	Image() = default;

	Image(class Device* device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);

	~Image();

	VkImage GetImage() const { return m_Image; }

	void BindToMemory(VkMemoryPropertyFlags properties);

private:

	class Device* m_Device = nullptr;

	VkImage m_Image = VK_NULL_HANDLE;
	VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;

};