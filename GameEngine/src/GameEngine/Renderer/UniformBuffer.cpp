#include "pch.h"
#include "UniformBuffer.h"
#include "Vulkan/VulkanUniformBuffer.h"
#include "RendererAPI.h"

namespace GE
{
	Ref<UniformBuffer> GE::UniformBuffer::Create(uint64_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: assert(false && "None is not supported"); return nullptr;
			case RendererAPI::API::Vulkan: return MakeRef<VulkanUniformBuffer>(size, binding);
		}

		assert(false && "Unknown API");
		return nullptr;
	}

}