#include "pch.h"
#include "VertexBuffer.h"
#include "Core/Graphics/VulkanContext.h"
#include "Vertex.h"
#include "Core/Graphics/Vulkan/Device.h"

VertexBuffer::VertexBuffer(class Device* device, uint32_t size, const struct Vertex* vertices)
	:m_Size(size) , m_Device(device)
{

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(Vertex) * size;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &m_VertexBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vertex Buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device->GetDevice(), m_VertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device->GetDevice(), m_VertexBuffer, m_VertexBufferMemory, 0);

	void* data;
	vkMapMemory(device->GetDevice(), m_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices, (size_t)bufferInfo.size);
	vkUnmapMemory(device->GetDevice(), m_VertexBufferMemory);
}

VertexBuffer::~VertexBuffer()
{
	vkDestroyBuffer(m_Device->GetDevice(), m_VertexBuffer, nullptr);
	vkFreeMemory(m_Device->GetDevice(), m_VertexBufferMemory, nullptr);
}

void VertexBuffer::Bind(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

