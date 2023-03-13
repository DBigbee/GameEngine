#pragma once

#include <vulkan/vulkan.h>

namespace GE
{
	struct SamplerProperties
	{
		VkFilter m_MagFilter = VK_FILTER_LINEAR;
		VkFilter m_MinFilter = VK_FILTER_LINEAR;
		VkSamplerAddressMode m_AddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT; //tiling
		VkBorderColor m_BorderColor;
	};

	class TextureSampler
	{
	public:

		TextureSampler(const SamplerProperties& properties = {});
		virtual ~TextureSampler();

		VkSampler GetSampler() const { return m_Sampler; }

	private:

		VkSampler m_Sampler = VK_NULL_HANDLE;
		Ref<class Device> m_Device;
	};
}