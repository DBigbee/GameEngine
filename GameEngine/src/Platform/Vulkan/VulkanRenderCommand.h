#pragma once

#include "Core.h"
#include "Renderer/RenderCommand.h"
#include "VulkanRendererAPI.h"

namespace GE
{
	class VulkanRenderCommand : public RenderCommand
	{
	public:
		static Ref<VulkanRendererAPI> GetVulkanRenderAPI()
		{
			return Cast_Ptr<VulkanRendererAPI>(GetRendererAPI());
		}
	};
}