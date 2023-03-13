#pragma once

#include "vulkan/vulkan.h"

namespace GE
{
	struct ImageViewProperties
	{
		VkFormat m_Format;
		uint32_t m_MipLevels = 1;
		VkImageAspectFlags m_AspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		VkImageViewType m_ViewType = VK_IMAGE_VIEW_TYPE_2D;
	};

	class ImageView
	{
	public:

		ImageView(VkImage image, const ImageViewProperties& properties = {});
		virtual ~ImageView();

		VkImageView GetImageView() const { return m_ImageView; }

	private:

		VkImageView m_ImageView = VK_NULL_HANDLE;
		Ref<class Device> m_Device = nullptr;
	};
}