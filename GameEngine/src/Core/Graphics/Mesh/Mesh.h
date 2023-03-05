#pragma once

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:

	Mesh();
	Mesh(class Device* device, const std::vector<struct Vertex>& vertices, const std::vector<uint32_t>& indices);
	virtual ~Mesh();

	void Draw(VkCommandBuffer commandBuffer);

private:

	std::unique_ptr<class VertexBuffer> m_VertexBuffer;

	std::unique_ptr<class IndexBuffer> m_IndexBuffer;
};