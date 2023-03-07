#include "pch.h"
#include "UniformBuffer.h"
#include "Core/Graphics/Vulkan/Device.h"


UniformBuffer::UniformBuffer(Device* device, VkDeviceSize size, uint32_t count)
	:Buffer(device, size, count, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device->GetProperties().limits.minUniformBufferOffsetAlignment)
{
	Map();
}
