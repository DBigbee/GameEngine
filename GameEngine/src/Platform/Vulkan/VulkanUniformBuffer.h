#pragma once

#include "Renderer/UniformBuffer.h"
#include "VulkanBuffer.h"

namespace GE
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(uint64_t size, uint32_t count);

		virtual void SetData(const void* data, uint64_t size, uint32_t offset = 0)  override;

		VkDescriptorBufferInfo GetBufferInfo()
		{
			return m_Buffer->GetDescriptorBufferInfo();
		}

	private:

		Scope<VulkanBuffer> m_Buffer;
	};
}