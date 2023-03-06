#include "pch.h"
#include "IndexBuffer.h"
#include "Core/Graphics/Vulkan/Device.h"

IndexBuffer::IndexBuffer(Device* device, uint32_t size, const uint32_t* indices)
	:Buffer(device), m_Size(size)
{
	auto bufferSize = sizeof(uint32_t) * size;
	VkBuffer StagingBuffer;
	VkDeviceMemory StagingBufferMemory;
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, StagingBuffer, StagingBufferMemory);

	void* data;
	vkMapMemory(device->GetDevice(), StagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices, (size_t)bufferSize);
	vkUnmapMemory(device->GetDevice(), StagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_Buffer, m_BufferMemory);

	CopyBuffer(StagingBuffer, m_Buffer, bufferSize);

	vkDestroyBuffer(device->GetDevice(), StagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), StagingBufferMemory, nullptr);
}

void IndexBuffer::Bind(VkCommandBuffer commandBuffer)
{
	vkCmdBindIndexBuffer(commandBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT32);
}
