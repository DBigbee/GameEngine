#include "pch.h"
#include "VulkanTexture2D.h"
#include "stb_image/stb_image.h"
#include "Device.h"
#include "Vulkan/VulkanBuffer.h"
#include "CommandBuffer.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	VulkanTexture2D::VulkanTexture2D(const std::string& path)
		:m_FilePath(path)
	{
		int width, height, channels;
		stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		VkDeviceSize imageSize = width * height * 4;



		if (!pixels)
		{
			throw std::runtime_error("failed to load texture image");
		}

		m_Width = width;
		m_Height = height;
		m_Channels = channels;
		m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

		auto& device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		VulkanBuffer stagingBuffer{ sizeof(uint8_t), (uint32_t)imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };
		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)pixels);

		stbi_image_free(pixels);

		m_Image = MakeScope<Image>(m_Width, m_Height, ImageProperties{ VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, m_MipLevels });
		m_Image->BindToMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		m_Image->TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_MipLevels);
		m_Image->CopyBufferToImage(stagingBuffer.GetBuffer());

		
		//m_Image->TransitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, m_MipLevels);
		m_Image->GenerateMipMaps(VK_FORMAT_R8G8B8A8_SRGB, m_MipLevels);

		m_ImageView = MakeScope<ImageView>(m_Image->GetImage(), ImageViewProperties{VK_FORMAT_R8G8B8A8_SRGB, m_MipLevels});

		m_Sampler = MakeScope<TextureSampler>(SamplerProperties{ m_MipLevels });
	}

	VkDescriptorImageInfo VulkanTexture2D::GetImageInfo() const
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = m_ImageView->GetImageView();
		imageInfo.sampler = m_Sampler->GetSampler();

		return imageInfo;
	}
}