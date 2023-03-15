#include "pch.h"
#include "TextureSampler.h"
#include "Device.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	TextureSampler::TextureSampler(const SamplerProperties& properties)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

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
		samplerInfo.maxLod = static_cast<float>(properties.m_MipLevels);
		
		if (vkCreateSampler(*m_Device, &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS)
		{
			GE_CORE_ASSERT(false, "failed to create texture sampler!");
		}
	}

	TextureSampler::~TextureSampler()
	{
		vkDestroySampler(*m_Device, m_Sampler, nullptr);
	}
}
