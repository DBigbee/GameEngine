#include "pch.h"
#include "VertexBuffer.h"
#include "Core/Graphics/VulkanContext.h"
#include "Vertex.h"
#include "Core/Graphics/Vulkan/Device.h"

VertexBuffer::VertexBuffer(class Device* device, uint32_t size, const struct Vertex* vertices)
	:Buffer(device, vertices, sizeof(Vertex)* size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT), m_Size(size)
{
	
}

void VertexBuffer::Bind(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_Buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

