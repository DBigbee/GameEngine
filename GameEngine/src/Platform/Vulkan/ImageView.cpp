#include "pch.h"
#include "ImageView.h"
#include "Image.h"
#include "Device.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	ImageView::ImageView(VkImage image, const ImageViewProperties& properties)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;

		createInfo.viewType = properties.m_ViewType;
		createInfo.format = properties.m_Format;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = properties.m_AspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = properties.m_MipLevels;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(*m_Device, &createInfo, nullptr, &m_ImageView) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image views!");
		}
	}

	ImageView::~ImageView()
	{
		vkDestroyImageView(*m_Device, m_ImageView, nullptr);
	}
}