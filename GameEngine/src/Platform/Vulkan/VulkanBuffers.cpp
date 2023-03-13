#include "pch.h"
#include "VulkanBuffers.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	VulkanIndexBuffer::VulkanIndexBuffer(const uint32_t* indices, uint32_t size)
	{
	
		m_Buffer = MakeScope<VulkanBuffer>(sizeof(uint32_t), size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VulkanBuffer stagingBuffer(sizeof(uint32_t), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)indices);

		auto& device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();
		device->CopyBuffer(stagingBuffer.GetBuffer(), m_Buffer->GetBuffer(), m_Buffer->GetBufferSize());
	}

	void VulkanIndexBuffer::Bind() const
	{
		auto cmd = VulkanRenderCommand::GetVulkanRenderAPI()->GetCommandBuffer();
		vkCmdBindIndexBuffer(*cmd, m_Buffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}
	void VulkanIndexBuffer::UnBind() const
	{
		
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const Vertex* vertices, uint32_t count)
	{
		m_Buffer = MakeScope<VulkanBuffer>(sizeof(Vertex), count, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VulkanBuffer stagingBuffer(sizeof(Vertex), count, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices);

		auto& device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();
		device->CopyBuffer(stagingBuffer.GetBuffer(), m_Buffer->GetBuffer(), m_Buffer->GetBufferSize());
	}

	void VulkanVertexBuffer::Bind() const
	{
		auto cmd = VulkanRenderCommand::GetVulkanRenderAPI()->GetCommandBuffer();
		VkBuffer vertexBuffers[] = { m_Buffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(*cmd, 0, 1, vertexBuffers, offsets);
	}

	void VulkanVertexBuffer::UnBind() const
	{

	}
}