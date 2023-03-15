#include "pch.h"
#include "VulkanBuffer.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	VulkanBuffer::VulkanBuffer(VkDeviceSize size, uint32_t count, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize minOffetAlignment)
		:m_Buffer(nullptr), m_BufferMemory(nullptr), m_Count(count), m_Size(size), m_UsageFlags(usage), m_PropertyFlags(properties)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		m_AlignmentSize = GetAlignment(size, minOffetAlignment);
		m_BufferSize = m_AlignmentSize * count;

		auto & device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();
		device->CreateBuffer(m_BufferSize, usage, properties, m_Buffer, m_BufferMemory);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		UnMap();

		vkDestroyBuffer(*m_Device, m_Buffer, nullptr);
		vkFreeMemory(*m_Device, m_BufferMemory, nullptr);
	}

	VkDescriptorBufferInfo VulkanBuffer::GetDescriptorBufferInfo(VkDeviceSize size, VkDeviceSize offset)
	{
		return VkDescriptorBufferInfo
		{
			m_Buffer,
			offset,
			size
		};
	}

	VkDescriptorBufferInfo VulkanBuffer::GetDescriptorBufferInfoForIndex(int index)
	{
		return GetDescriptorBufferInfo(m_AlignmentSize, index * m_AlignmentSize);
	}

	VkResult VulkanBuffer::Map(VkDeviceSize size, VkDeviceSize offset)
	{
		GE_CORE_ASSERT(m_Buffer && m_BufferMemory , "Called map on buffer before create!");


		return vkMapMemory(*m_Device, m_BufferMemory, offset, size, 0, &m_MappedBuffers);
	}

	void VulkanBuffer::UnMap()
	{
		if (m_MappedBuffers)
		{
			
			vkUnmapMemory(*m_Device, m_BufferMemory);
			m_MappedBuffers = nullptr;
		}
	}


	void VulkanBuffer::WriteToBuffer(const void* data, VkDeviceSize size, VkDeviceSize offset)
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

	void VulkanBuffer::WriteToIndex(const void* data, int index)
	{
		WriteToBuffer(data, m_Size, index * m_AlignmentSize);
	}

	VkResult VulkanBuffer::Flush(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_BufferMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		return vkFlushMappedMemoryRanges(*m_Device, 1, &mappedRange);
	}

	VkResult VulkanBuffer::FlushIndex(int index)
	{
		return Flush(m_AlignmentSize, index * m_AlignmentSize);
	}

	VkResult VulkanBuffer::Invalidate(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_BufferMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		return vkInvalidateMappedMemoryRanges(*m_Device, 1, &mappedRange);
	}

	VkResult VulkanBuffer::InvalidateIndex(int index)
	{
		return Invalidate(m_AlignmentSize, index * m_AlignmentSize);
	}

	VkDeviceSize VulkanBuffer::GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
	{
		if (minOffsetAlignment > 0) {
			return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return instanceSize;
	}
}