#include "pch.h"
#include "Texture2D.h"
#include "Core/ImageLoader/StbImage.h"
#include <vulkan/vulkan.h>

Texture2D::Texture2D(const std::string& filePath)
	:m_FilePath(filePath)
{
	int width, height, channels;
	stbi_uc* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	VkDeviceSize size = width * height * 4;

	m_Width = width;
	m_Height = height;
	m_Channels = channels;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image");
	}

	Invalidate(pixels);

	stbi_image_free(pixels);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Invalidate(uint8_t* data)
{
}
