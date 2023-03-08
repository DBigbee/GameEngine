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


	CommandBuffer commandBuffer(device, 1, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	TransitionImageLayout(&commandBuffer, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(&commandBuffer, stagingBuffer.GetBuffer());
	TransitionImageLayout(&commandBuffer, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	m_ImageView = std::make_unique<ImageView>(device, m_Image.get(), VK_FORMAT_R8G8B8A8_SRGB);

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

void Texture2D::TransitionImageLayout(CommandBuffer* commandBuffer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	commandBuffer->Begin();

	VkPipelineStageFlags sourceStage{};
	VkPipelineStageFlags destinationStage{};

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = m_Image->GetImage();
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.levelCount = 1;


	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0; // TODO
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // TODO

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}


	vkCmdPipelineBarrier(commandBuffer->GetBuffer(), sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	commandBuffer->End();
	commandBuffer->Submit();
}

void Texture2D::CopyBufferToImage(CommandBuffer* commandBuffer, VkBuffer buffer)
{
	commandBuffer->Begin();

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0 ,0 };
	region.imageExtent = { m_Width , m_Height, 1 };

	vkCmdCopyBufferToImage(commandBuffer->GetBuffer(), buffer, m_Image->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	commandBuffer->End();
	commandBuffer->Submit();
}

void Texture2D::Invalidate(uint8_t* data)
{
}
