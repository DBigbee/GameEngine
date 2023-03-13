#pragma once

#include <vulkan/vulkan.h>

namespace GE
{

	class Image
	{
	public:

		Image() = default;

		Image(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);

		virtual ~Image();


		void BindToMemory(VkMemoryPropertyFlags properties);

		void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void CopyBufferToImage(VkBuffer buffer);

		VkImage GetImage() const { return m_Image; }

	private:

		uint32_t m_Width;

		uint32_t m_Height;

		VkImage m_Image = VK_NULL_HANDLE;

		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;

		Ref<class Device> m_Device;
	};
}