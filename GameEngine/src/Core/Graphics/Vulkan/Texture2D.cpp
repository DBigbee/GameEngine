#include "pch.h"
#include "Texture2D.h"
#include "Core/ImageLoader/StbImage.h"
#include "Buffer.h"
#include "Device.h"
#include "Image.h"
#include "ImageView.h"
#include "Buffer.h"
#include "CommandBuffer.h"
#include "TextureSampler.h"

Texture2D::Texture2D(class Device* device, const std::string& filePath)
	:m_FilePath(filePath)
{
	int width, height, channels;
	stbi_uc* pixels = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	VkDeviceSize imageSize = width * height * 4;

	m_Width = width;
	m_Height = height;
	m_Channels = channels;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image");
	}

	Buffer stagingBuffer{ device, sizeof(uint8_t), (uint32_t)imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };
	stagingBuffer.Map();
	stagingBuffer.WriteToBuffer((void*)pixels);

	stbi_image_free(pixels);

	m_Image = std::make_unique<Image>(device, m_Width, m_Height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	m_Image->BindToMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);



	m_Image->TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	m_Image->CopyBufferToImage(stagingBuffer.GetBuffer());
	m_Image->TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	m_ImageView = std::make_unique<ImageView>(device, m_Image.get(), ImageViewProperties{VK_FORMAT_R8G8B8A8_SRGB });

	m_Sampler = std::make_unique<TextureSampler>(device);
}

Texture2D::~Texture2D()
{

}

VkDescriptorImageInfo Texture2D::GetImageInfo() const
{
	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = m_ImageView->GetImageView();
	imageInfo.sampler = m_Sampler->GetSampler();

	return imageInfo;
}


void Texture2D::Invalidate(uint8_t* data)
{
}
