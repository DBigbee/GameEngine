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
			assert(false && "None is not supported");
			return nullptr;
		case RendererAPI::API::Vulkan:
			return MakeRef<VulkanShader>(filename);
		}

		assert(false && "Unknown API");
		return nullptr;
	}
}
