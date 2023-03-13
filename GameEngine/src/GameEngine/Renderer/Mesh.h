#pragma once

#include "Buffers.h"
#include "VertexArray.h"
#include "Vertex.h"

namespace GE
{
	class Mesh
	{
	public:

		Mesh();
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		virtual ~Mesh();

		Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

	private:

		Ref<VertexArray> m_VertexArray;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexBuffer> m_VertexBuffer;
	};
}