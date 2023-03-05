#pragma once

#include <vulkan/vulkan.h>
#include "Vertex.h"

class VertexBuffer final
{
public:
	VertexBuffer(class Device* device,  uint32_t size, const struct Vertex* vertices);

	~VertexBuffer();

	void Bind(VkCommandBuffer commandBuffer);

	uint32_t GetSize() const { return m_Size; }

private:

	VkBuffer m_VertexBuffer;

	VkDeviceMemory m_VertexBufferMemory;

	uint32_t m_Size;

	class Device* m_Device = nullptr;
};

//class IndexBuffer final
//{
//public:
//	IndexBuffer();
//
//	~IndexBuffer();
//};