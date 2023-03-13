#pragma once

#include "Vertex.h"

namespace GE
{
	class VertexBuffer
	{
	public:

		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Ref<VertexBuffer> Create(const Vertex* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t size);
	};
}