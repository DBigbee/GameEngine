#include "pch.h"
#include "TextureSampler.h"
#include "Device.h"

TextureSampler::TextureSampler(Device* device, const SamplerProperties& properties)
	:m_Device(device)
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = properties.m_MagFilter;
	samplerInfo.minFilter = properties.m_MinFilter;
	samplerInfo.addressModeU = properties.m_AddressMode;
	samplerInfo.addressModeV = properties.m_AddressMode;
	samplerInfo.addressModeW = properties.m_AddressMode;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 1.0f;
	samplerInfo.borderColor = properties.m_BorderColor;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

TextureSampler::~TextureSampler()
{
	vkDestroySampler(m_Device->GetDevice(), m_Sampler, nullptr);
}
