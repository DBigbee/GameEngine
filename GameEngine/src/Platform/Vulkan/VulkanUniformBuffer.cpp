#include "pch.h"
#include "VulkanUniformBuffer.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint64_t size, uint32_t count)
	{
		auto& device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		m_Buffer = MakeScope<VulkanBuffer>(size, count, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device->GetProperties().limits.minUniformBufferOffsetAlignment);

		m_Buffer->Map();
	}

	void VulkanUniformBuffer::SetData(const void* data, uint64_t size, uint32_t offset)
	{
		m_Buffer->WriteToBuffer(data, size, offset);
	}
}

