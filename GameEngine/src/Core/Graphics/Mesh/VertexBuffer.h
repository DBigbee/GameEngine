#pragma once

#include "Vertex.h"
#include "Buffer.h"

class VertexBuffer final : public Buffer
{
public:
	VertexBuffer(class Device* device,  uint32_t count, const struct Vertex* vertices);
};
