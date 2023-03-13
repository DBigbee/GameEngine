#include "pch.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Vulkan/VulkanVertexArray.h"

namespace GE
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: assert(false && "None is not supported"); return nullptr;
		case RendererAPI::API::Vulkan: return MakeRef<VulkanVertexArray>();
		}

		assert(false && "Unknown API");
		return nullptr;
	}
}