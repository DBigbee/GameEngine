#include "pch.h"
#include "Mesh.h"

#include "vulkan/vulkan.h"

Mesh::Mesh()
{
}

Mesh::Mesh(class Device* device, const std::vector<struct Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_VertexBuffer = std::make_unique<VertexBuffer>(device, static_cast<uint32_t>(vertices.size()), vertices.data());
	m_IndexBuffer = std::make_unique<IndexBuffer>(device, static_cast<uint32_t>(indices.size()), indices.data());
}

Mesh::~Mesh()
{
	
}

void Mesh::Draw(VkCommandBuffer commandBuffer)
{
	if (!m_IndexBuffer)
	{
		vkCmdDraw(commandBuffer, m_VertexBuffer->GetCount(), 1, 0, 0);
	}
	else
	{
		
		vkCmdDrawIndexed(commandBuffer, m_IndexBuffer->GetCount(), 1, 0, 0, 0);
	}
}

void Mesh::Bind(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()};
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	if (m_IndexBuffer)
	{
		vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}
}

