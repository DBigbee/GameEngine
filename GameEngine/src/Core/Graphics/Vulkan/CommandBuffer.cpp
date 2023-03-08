#include "pch.h"
#include "CommandBuffer.h"
#include "Device.h"

CommandBuffer::CommandBuffer(class Device* device, uint32_t count, VkCommandBufferUsageFlags usage)
	:m_Device(device), m_Count(count) , m_Usage(usage)
{
	m_CommandBuffers.resize(count);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = device->GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = count;
	

	if (vkAllocateCommandBuffers(device->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command buffers!");
	}
}

CommandBuffer::~CommandBuffer()
{
	Free();
}

VkCommandBuffer CommandBuffer::Begin(int index)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = m_Usage;
	beginInfo.pInheritanceInfo = nullptr;

	m_CurrentCommandBuffer = m_CommandBuffers[index];
	if (vkBeginCommandBuffer(m_CurrentCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording commandbuffer!");
	}

	return m_CurrentCommandBuffer;
}

void CommandBuffer::End()
{
	if (vkEndCommandBuffer(m_CurrentCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffers!");
	}
}

void CommandBuffer::Free()
{
	vkFreeCommandBuffers(m_Device->GetDevice(), m_Device->GetCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	m_CommandBuffers.clear();
}

VkCommandBuffer CommandBuffer::GetAtIndex(int index)
{
	return m_CommandBuffers[index];
}
