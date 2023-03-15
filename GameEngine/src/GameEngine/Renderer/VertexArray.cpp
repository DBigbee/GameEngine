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
		case RendererAPI::API::None:
			GE_CORE_ASSERT(false , "None is not supported");
			return nullptr;
		case RendererAPI::API::Vulkan: return MakeRef<VulkanVertexArray>();
		}

		GE_CORE_ASSERT(false , "Unknown API");
		return nullptr;
	}
}