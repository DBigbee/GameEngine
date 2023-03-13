#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/Buffers.h"


namespace GE
{
	class VulkanVertexArray : public VertexArray
	{
	public:
		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const override { return m_VertexBuffer; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}

