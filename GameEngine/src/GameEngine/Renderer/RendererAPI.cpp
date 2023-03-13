#include "pch.h"
#include "RendererAPI.h"
#include "Vulkan/VulkanRendererAPI.h"

namespace GE
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;

	Ref<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None: assert(false && "None is not supported"); return nullptr;
		case RendererAPI::API::Vulkan: return MakeRef<VulkanRendererAPI>();
		}

		assert(false && "Unknown API");
		return nullptr;
	}
}