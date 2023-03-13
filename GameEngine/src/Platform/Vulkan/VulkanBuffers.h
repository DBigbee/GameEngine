#pragma once

#include "Renderer/Buffers.h"
#include "VulkanBuffer.h"

namespace GE
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:

		VulkanIndexBuffer(const uint32_t* indices, uint32_t size);

		void Bind() const override;

		void UnBind() const override;

		uint32_t GetCount() const override { return m_Buffer->GetCount(); }

	private:

		Scope<VulkanBuffer> m_Buffer;
	};

	class VulkanVertexBuffer final : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(const Vertex* vertices, uint32_t size);

		void Bind() const override;
		void UnBind() const override;

	private:

		Scope<VulkanBuffer> m_Buffer;
	};
}