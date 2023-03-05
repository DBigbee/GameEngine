#include "pch.h"
#include "IndexBuffer.h"
#include "Core/Graphics/Vulkan/Device.h"

IndexBuffer::IndexBuffer(Device* device, uint32_t size, const uint32_t* indices)
	:Buffer(device, indices, sizeof(uint32_t) * size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT), m_Size(size)
{
	
}

void IndexBuffer::Bind(VkCommandBuffer commandBuffer)
{
	vkCmdBindIndexBuffer(commandBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT32);
}
