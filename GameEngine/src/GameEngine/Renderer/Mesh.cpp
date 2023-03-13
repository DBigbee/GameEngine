#include "pch.h"
#include "Mesh.h"

namespace GE
{
	Mesh::Mesh()
	{
	}

	Mesh::Mesh(const std::vector<struct Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		m_IndexBuffer = IndexBuffer::Create(indices.data(), (uint32_t)indices.size());
		m_VertexBuffer = VertexBuffer::Create( vertices.data(), (uint32_t)vertices.size());

		m_VertexArray = VertexArray::Create();
		m_VertexArray->SetVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	Mesh::~Mesh()
	{

	}

}