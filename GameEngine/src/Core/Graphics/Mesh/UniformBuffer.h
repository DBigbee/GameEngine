#pragma once

#include "Buffer.h"

class UniformBuffer : public Buffer
{
public:
	UniformBuffer(class Device* device, VkDeviceSize size, uint32_t count);

};