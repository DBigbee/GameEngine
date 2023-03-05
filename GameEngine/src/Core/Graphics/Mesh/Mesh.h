#pragma once

#include "Vertex.h"
#include "VertexBuffer.h"

class Mesh
{
public:

	Mesh();
	Mesh(class Device* device,const std::vector<struct Vertex>& vertices);
	virtual ~Mesh();

	void Draw(VkCommandBuffer commandBuffer);

private:

	std::shared_ptr<class VertexBuffer> m_VertexBuffer;
};