#pragma once

#include <vulkan/vulkan.h>

class Buffer
{
public:

	Buffer(class Device* device,  VkDeviceSize size, uint32_t count, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceSize minOffetAlignment = 1);

	virtual ~Buffer();

	class Device* GetDevice() { return m_Device; }

	virtual void Bind(VkCommandBuffer commandBuffer) {}

	VkBuffer GetBuffer() const { return m_Buffer; }

	VkDescriptorBufferInfo GetDescriptorBufferInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	VkDescriptorBufferInfo GetDescriptorBufferInfoForIndex(int index);

	VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	void UnMap();


	void WriteToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	void WriteToIndex(void* data, int index);

	VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	VkResult FlushIndex(int index);

	VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	VkResult InvalidateIndex(int index);

	uint32_t GetCount() const { return m_Count; }

protected:
	VkBuffer m_Buffer = VK_NULL_HANDLE;

	VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;

	void* m_MappedBuffers = nullptr;

	uint32_t m_Count = 0;

	VkDeviceSize m_AlignmentSize;

	VkDeviceSize m_BufferSize = 0;

	VkDeviceSize m_Size;

	VkBufferUsageFlags m_UsageFlags;

	VkMemoryPropertyFlags m_PropertyFlags;

private:

	static VkDeviceSize GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

	class Device* m_Device = nullptr;
};