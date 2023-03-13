#include "pch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Vulkan/VulkanTexture2D.h"

namespace GE
{

	Ref<Texture2D> GE::Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None: assert(false && "None is not supported"); return nullptr;
		case RendererAPI::API::Vulkan: return MakeRef<VulkanTexture2D>(path);
		}

		assert(false && "Unknown API");
		return nullptr;
	}
}
