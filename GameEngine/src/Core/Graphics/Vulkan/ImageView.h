#pragma once

#include "vulkan/vulkan.h"

class ImageView
{
public:

	ImageView(class Device* device, class Image* image, VkFormat format, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
	virtual ~ImageView();

private:

	VkImageView m_ImageView = VK_NULL_HANDLE;
	class Device* m_Device = nullptr;
};