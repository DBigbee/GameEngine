#pragma once

#include <vulkan/vulkan.h>

class Buffer
{
public:

	Buffer(class Device* device);

	virtual ~Buffer();

	class Device* GetDevice() { return m_Device; }

	virtual void Bind(VkCommandBuffer commandBuffer) {}

	VkBuffer GetBuffer() const { return m_Buffer; }

protected:
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);

protected:
	VkBuffer m_Buffer;

	VkDeviceMemory m_BufferMemory;


private:

	class Device* m_Device = nullptr;
};