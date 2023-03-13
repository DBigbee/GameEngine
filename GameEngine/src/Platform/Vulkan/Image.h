#pragma once

#include <vulkan/vulkan.h>

namespace GE
{
	struct ImageProperties
	{
		VkFormat m_Format;
		VkImageTiling m_Tiling;
		VkImageUsageFlags m_Usage;
		uint32_t m_MipLevels = 1;
		VkSampleCountFlagBits m_NumSamples = VK_SAMPLE_COUNT_1_BIT;
	};

	class Image
	{
	public:

		Image() = default;

		Image(uint32_t width, uint32_t height, const ImageProperties& properties);

		virtual ~Image();


		void BindToMemory(VkMemoryPropertyFlags properties);

		void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout , uint32_t mipLevels = 1);

		void CopyBufferToImage(VkBuffer buffer);

		void GenerateMipMaps(VkFormat imageFormat, uint32_t mipLevels);

		VkImage GetImage() const { return m_Image; }

	private:

		uint32_t m_Width;

		uint32_t m_Height;

		VkImage m_Image = VK_NULL_HANDLE;

		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;

		Ref<class Device> m_Device;
	};
}