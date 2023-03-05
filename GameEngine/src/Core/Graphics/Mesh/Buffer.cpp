#include "pch.h"
#include "Buffer.h"
#include "Core/Graphics/Vulkan/Device.h"

Buffer::Buffer(Device* device, const void* srcdata, VkDeviceSize size, VkBufferUsageFlags usage)
	:m_Device(device)
{
	VkBuffer StagingBuffer;
	VkDeviceMemory StagingBufferMemory;
	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, StagingBuffer, StagingBufferMemory);

	void* data;
	vkMapMemory(device->GetDevice(), StagingBufferMemory, 0, size, 0, &data);
	memcpy(data, srcdata, (size_t)size);
	vkUnmapMemory(device->GetDevice(), StagingBufferMemory);

	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_Buffer, m_BufferMemory);

	CopyBuffer(StagingBuffer, m_Buffer, size);

	vkDestroyBuffer(device->GetDevice(), StagingBuffer, nullptr);
	vkFreeMemory(device->GetDevice(), StagingBufferMemory, nullptr);
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
