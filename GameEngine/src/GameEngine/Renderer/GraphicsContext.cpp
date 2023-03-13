#include "pch.h"
#include "GraphicsContext.h"
#include "RendererAPI.h"
#include "Vulkan/VulkanContext.h"

namespace GE
{
	Ref<GraphicsContext> GraphicsContext::Create(struct GLFWwindow* window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: assert(false && "None is not supported"); return nullptr;
		case RendererAPI::API::Vulkan: return MakeRef<VulkanContext>(window);
		}

		assert(false && "Unknown API");
		return nullptr;
	}
}

