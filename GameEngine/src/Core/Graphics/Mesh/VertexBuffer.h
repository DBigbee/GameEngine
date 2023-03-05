#pragma once

#include "Vertex.h"
#include "Buffer.h"

class VertexBuffer final : public Buffer
{
public:
	VertexBuffer(class Device* device,  uint32_t size, const struct Vertex* vertices);

	void Bind(VkCommandBuffer commandBuffer) override;

	uint32_t GetSize() const { return m_Size; }

private:

	
	uint32_t m_Size;
};

//class IndexBuffer final
//{
//public:
//	IndexBuffer();
//
//	~IndexBuffer();
//};