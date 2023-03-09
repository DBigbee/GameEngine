#pragma once

#include <vulkan/vulkan.h>

class CommandBuffer;

class Image
{
public:

	Image() = default;

	Image(class Device* device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);

	~Image();

	VkImage GetImage() const { return m_Image; }

	VkDeviceMemory GetMemory() const { return m_ImageMemory; }

	void BindToMemory(VkMemoryPropertyFlags properties);

	void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void CopyBufferToImage(VkBuffer buffer);

private:
	class Device* m_Device = nullptr;

	uint32_t m_Width;

	uint32_t m_Height;

	VkImage m_Image = VK_NULL_HANDLE;

	VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;

};