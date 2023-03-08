#pragma once

#include <vulkan/vulkan.h>

class CommandBuffer
{
public:

	CommandBuffer(class Device* device, uint32_t count, VkCommandBufferUsageFlags usage);
	~CommandBuffer();

	VkCommandBuffer Begin(int index = 0);

	void End();

	void Free();

	VkCommandBuffer GetAtIndex(int index);

	VkCommandBuffer GetCurrent() const { return m_CurrentCommandBuffer; }

	std::vector<VkCommandBuffer> GetBuffers() const { return m_CommandBuffers; }

	uint32_t GetCount() { return m_Count; }

private:

	VkCommandBuffer m_CurrentCommandBuffer;

	std::vector<VkCommandBuffer> m_CommandBuffers;

	class Device* m_Device = nullptr;

	uint32_t m_Count = 0;

	VkCommandBufferUsageFlags m_Usage;
};