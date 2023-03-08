#include "pch.h"
#include "Buffer.h"
#include "Core/Graphics/Vulkan/Device.h"


Buffer::Buffer(Device* device,  VkDeviceSize size, uint32_t count, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize minOffetAlignment)
	:m_Buffer(nullptr), m_BufferMemory(nullptr), m_Device(device), m_Count(count), m_Size(size), m_UsageFlags(usage), m_PropertyFlags(properties)
{
	m_AlignmentSize = GetAlignment(size, minOffetAlignment);
	m_BufferSize = m_AlignmentSize * count;
	device->CreateBuffer(m_BufferSize, usage, properties, m_Buffer, m_BufferMemory);
}

Buffer::~Buffer()
{
	UnMap();
	vkDestroyBuffer(GetDevice()->GetDevice(), m_Buffer, nullptr);
	vkFreeMemory(GetDevice()->GetDevice(), m_BufferMemory, nullptr);
}

VkDescriptorBufferInfo Buffer::GetDescriptorBufferInfo(VkDeviceSize size, VkDeviceSize offset)
{
	return VkDescriptorBufferInfo
	{
		m_Buffer,
		offset,
		size
	};
}

VkDescriptorBufferInfo Buffer::GetDescriptorBufferInfoForIndex(int index)
{
	return GetDescriptorBufferInfo(m_AlignmentSize, index * m_AlignmentSize);
}

VkResult Buffer::Map(VkDeviceSize size, VkDeviceSize offset)
{
	assert(m_Buffer && m_BufferMemory && "Called map on buffer before create!");
	return vkMapMemory(m_Device->GetDevice(), m_BufferMemory, offset, size, 0, &m_MappedBuffers);
}

void Buffer::UnMap()
{
	if (m_MappedBuffers)
	{
		vkUnmapMemory(m_Device->GetDevice(), m_BufferMemory);
		m_MappedBuffers = nullptr;
	}
}


void Buffer::WriteToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset)
{
	if (size == VK_WHOLE_SIZE)
	{
		memcpy(m_MappedBuffers, data, m_BufferSize);
	}
	else
	{
		char* memOffset = (char*)m_MappedBuffers;
		memOffset += offset;
		memcpy(memOffset, data, size);
	}
	
}

void Buffer::WriteToIndex(void* data, int index)
{
	WriteToBuffer(data, m_Size, index * m_AlignmentSize);
}

VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = m_BufferMemory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkFlushMappedMemoryRanges(m_Device->GetDevice(), 1, &mappedRange);
}

VkResult Buffer::FlushIndex(int index)
{
	return Flush(m_AlignmentSize, index * m_AlignmentSize);
}

VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = {};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = m_BufferMemory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkInvalidateMappedMemoryRanges(m_Device->GetDevice(), 1, &mappedRange);
}

VkResult Buffer::InvalidateIndex(int index)
{
	return Invalidate(m_AlignmentSize, index * m_AlignmentSize);
}

VkDeviceSize Buffer::GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
{
	if (minOffsetAlignment > 0) {
		return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
	}
	return instanceSize;
}
