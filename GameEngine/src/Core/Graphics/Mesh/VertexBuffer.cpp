#include "pch.h"
#include "VertexBuffer.h"
#include "Core/Graphics/VulkanContext.h"
#include "Vertex.h"
#include "Core/Graphics/Vulkan/Device.h"

VertexBuffer::VertexBuffer(class Device* device, uint32_t count, const struct Vertex* vertices)
	:Buffer(device, sizeof(Vertex), count, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{

	Buffer stagingBuffer( device, sizeof(Vertex), count, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer.Map();
	stagingBuffer.WriteToBuffer((void*)vertices);


	device->CopyBuffer(stagingBuffer.GetBuffer(), GetBuffer(), m_BufferSize);
}

