#pragma once

#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:

	IndexBuffer(class Device* device, uint32_t count, const uint32_t* indices);
};