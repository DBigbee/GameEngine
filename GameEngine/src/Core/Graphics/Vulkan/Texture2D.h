#pragma once

#include <vulkan/vulkan.h>

class CommandBuffer;

class Texture2D
{
public:

	Texture2D(class Device* device, const std::string& filePath);

	virtual ~Texture2D();

	int32_t GetWidth() const { return m_Width; }

	int32_t GetHeight() const { return m_Height; }

	int32_t GetChannels() const { return m_Channels; }

	VkDescriptorImageInfo GetImageInfo() const;

	std::string GetFilePath() const { return m_FilePath; }

private:

	void Invalidate(uint8_t* data);

	uint32_t m_Width = 0;

	uint32_t m_Height = 0;

	uint32_t m_Channels = 0;

	std::string m_FilePath;

	std::unique_ptr<class Image> m_Image = nullptr;

	std::unique_ptr<class ImageView> m_ImageView = nullptr;

	std::unique_ptr<class TextureSampler> m_Sampler = nullptr;
};