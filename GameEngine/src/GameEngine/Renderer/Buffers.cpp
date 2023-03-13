#include "pch.h"
#include "Buffers.h"
#include "RendererAPI.h"
#include "Vulkan/VulkanBuffers.h"

namespace GE
{
	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: assert(false && "None is not supported"); return nullptr;
			case RendererAPI::API::Vulkan: return MakeRef<VulkanIndexBuffer>(indices, size);
		}

		assert(false && "Unknown API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const Vertex* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: assert(false && "None is not supported"); return nullptr;
			case RendererAPI::API::Vulkan: return MakeRef<VulkanVertexBuffer>(vertices, size);
		}

		assert(false && "Unknown API");
		return nullptr;
	}
}