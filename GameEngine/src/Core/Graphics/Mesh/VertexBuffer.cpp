#include "pch.h"
#include "VertexBuffer.h"
#include "Core/Graphics/VulkanContext.h"
#include "Vertex.h"
#include "Core/Graphics/Vulkan/Device.h"

VertexBuffer::VertexBuffer(class Device* device, uint32_t size, const struct Vertex* vertices)
	:Buffer(device), m_Size(size)
{
	auto bufferSize = sizeof(Vertex) * size;
	VkBuffer StagingBuffer;
	VkDeviceMemory StagingBufferMemory;
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, StagingBuffer, StagingBufferMemory);

	void* data;
	vkMapMemory(device->GetDevice(), StagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices, (size_t)bufferSize);
	vkUnmapMemory(device->GetDevice(), StagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_Buffer, m_BufferMemory);

	CopyBuffer(StagingBuffer, m_Buffer, bufferSize);

	vkDestroyBuffer(device->GetDevice(), StagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), StagingBufferMemory, nullptr);
}

void VertexBuffer::Bind(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_Buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

