#include "pch.h"
#include "Mesh.h"

#include "vulkan/vulkan.h"

Mesh::Mesh()
{
}

Mesh::Mesh(class Device* device, const std::vector<struct Vertex>& vertices)
{
	m_VertexBuffer = std::make_shared<VertexBuffer>(device, static_cast<uint32_t>(vertices.size()), vertices.data());
}

Mesh::~Mesh()
{
	
}

void Mesh::Draw(VkCommandBuffer commandBuffer)
{
	m_VertexBuffer->Bind(commandBuffer);

	vkCmdDraw(commandBuffer, m_VertexBuffer->GetSize(), 1, 0, 0);
}
