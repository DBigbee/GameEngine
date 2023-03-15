#include "pch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "Vulkan/VulkanShader.h"

namespace GE
{
	std::string Shader::ReadFile(const std::string& filename)
	{
		return std::string();
	}

	Ref<Shader> Shader::Create(const std::string& filename)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			GE_CORE_ASSERT(false , "None is not supported");
			return nullptr;
		case RendererAPI::API::Vulkan:
			return MakeRef<VulkanShader>(filename);
		}

		GE_CORE_ASSERT(false , "Unknown API");
		return nullptr;
	}
}
