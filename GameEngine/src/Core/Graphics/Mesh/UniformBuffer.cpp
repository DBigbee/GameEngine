#include "pch.h"
#include "UniformBuffer.h"
#include "Core/Graphics/Vulkan/Device.h"

UniformBuffer::UniformBuffer(class Device* device, uint32_t size)
	:Buffer(device)
{
	auto bufferSize = size;
	
	CreateBuffer(bufferSize,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer, m_BufferMemory);

	vkMapMemory(device->GetDevice(), m_BufferMemory, 0, bufferSize, 0, &m_MappedUniformBuffers);
}

void UniformBuffer::Update(uint32_t size, void* data)
{
	memcpy(m_MappedUniformBuffers, data, size);
}
