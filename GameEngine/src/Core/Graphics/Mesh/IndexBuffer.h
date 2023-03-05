#pragma once

#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:

	IndexBuffer(class Device* device, uint32_t size, const uint32_t* indices);

	void Bind(VkCommandBuffer commandBuffer) override;

	uint32_t GetSize() const { return m_Size; }
private:

	uint32_t m_Size;
};