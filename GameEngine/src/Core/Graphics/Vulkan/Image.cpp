#include "pch.h"
#include "Image.h"
#include "Device.h"

Image::Image(Device* device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage)
	:m_Device(device)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;

	if (vkCreateImage(device->GetDevice(), &imageInfo, nullptr, &m_Image) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create image!");
	}
}

Image::~Image()
{
	vkDestroyImage(m_Device->GetDevice(), m_Image, nullptr);
	vkFreeMemory(m_Device->GetDevice(), m_ImageMemory, nullptr);
}

void Image::BindToMemory(VkMemoryPropertyFlags properties)
{
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_Device->GetDevice(), m_Image, &memRequirements);

	m_Device->AllocateMemory(memRequirements.size, memRequirements.memoryTypeBits, m_ImageMemory, properties);

	vkBindImageMemory(m_Device->GetDevice(), m_Image, m_ImageMemory, 0);
}

