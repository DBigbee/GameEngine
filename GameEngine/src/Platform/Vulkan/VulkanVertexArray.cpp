#include "pch.h"
#include "VulkanVertexArray.h"

namespace GE
{
	void VulkanVertexArray::Bind()
	{
		m_VertexBuffer->Bind();

		if (m_IndexBuffer)
		{
			m_IndexBuffer->Bind();
		}
	}

	void VulkanVertexArray::UnBind()
	{

	}
	void VulkanVertexArray::SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		m_VertexBuffer = vertexBuffer;
	}
	void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
}