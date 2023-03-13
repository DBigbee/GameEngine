#pragma once

#include "Renderer/Texture.h"
#include "TextureSampler.h"
#include "Image.h"
#include "ImageView.h"

struct VkDescriptorImageInfo;

namespace GE
{
	class CommandBuffer;

	class VulkanTexture2D : public Texture2D
	{
	public:

		VulkanTexture2D(const std::string& path);

		int32_t GetWidth() const { return m_Width; }

		int32_t GetHeight() const { return m_Height; }

		int32_t GetChannels() const { return m_Channels; }

		VkDescriptorImageInfo GetImageInfo() const;

		std::string GetPath() const { return m_FilePath; }

	private:

		void Invalidate(uint8_t* data);

		uint32_t m_Width = 0;

		uint32_t m_Height = 0;

		uint32_t m_Channels = 0;

		std::string m_FilePath;

		Scope<class Image> m_Image = nullptr;
		
		Scope<class ImageView> m_ImageView = nullptr;
		
		Scope<class TextureSampler> m_Sampler = nullptr;
	};
}