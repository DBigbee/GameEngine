#include "pch.h"
#include "Buffer.h"
#include "Core/Graphics/Vulkan/Device.h"

Buffer::Buffer(Device* device)
	:m_Device(device)
{
	
}

Buffer::~Buffer()
{
	vkDestroyBuffer(GetDevice()->GetDevice(), m_Buffer, nullptr);
	vkFreeMemory(GetDevice()->GetDevice(), m_BufferMemory, nullptr);
}

void Buffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(GetDevice()->GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(GetDevice()->GetDevice(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = GetDevice()->FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(GetDevice()->GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate buffer memory!");
	}

	vkBindBufferMemory(GetDevice()->GetDevice(), buffer, bufferMemory, 0);
}

void Buffer::CopyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = GetDevice()->GetCommandPool();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(GetDevice()->GetDevice(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_Device->GetGraphicsQueue());
	vkFreeCommandBuffers(m_Device->GetDevice(), m_Device->GetCommandPool(), 1, &commandBuffer);
}
