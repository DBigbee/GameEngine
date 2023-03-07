#include "pch.h"
#include "IndexBuffer.h"
#include "Core/Graphics/Vulkan/Device.h"

IndexBuffer::IndexBuffer(Device* device, uint32_t count, const uint32_t* indices)
	:Buffer(device, sizeof(uint32_t), count, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{

	Buffer stagingBuffer(device, sizeof(uint32_t), count, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer.Map();
	stagingBuffer.WriteToBuffer((void*)indices);


	device->CopyBuffer(stagingBuffer.GetBuffer(), GetBuffer(), m_BufferSize);
}
