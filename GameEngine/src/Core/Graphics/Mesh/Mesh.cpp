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
	m_VertexBuffer->Bind(commandBuffer);
	m_IndexBuffer->Bind(commandBuffer);
	//vkCmdDraw(commandBuffer, m_VertexBuffer->GetSize(), 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer, m_IndexBuffer->GetSize(), 1, 0, 0, 0);
}
