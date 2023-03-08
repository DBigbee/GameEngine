#include "pch.h"
#include "ImageView.h"
#include "Image.h"
#include "Device.h"

ImageView::ImageView(class Device* device, class Image* image, VkFormat format, VkImageViewType viewType)
	:m_Device(device)
{
	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image->GetImage();

	createInfo.viewType = viewType;
	createInfo.format = format;

	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(device->GetDevice(), &createInfo, nullptr, &m_ImageView) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create image views!");
	}
}

ImageView::~ImageView()
{
	vkDestroyImageView(m_Device->GetDevice(), m_ImageView, nullptr);
}
