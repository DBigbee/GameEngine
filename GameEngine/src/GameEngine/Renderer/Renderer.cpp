#include "pch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Vulkan/VulkanRenderer.h"

namespace GE
{
	void Renderer::Init()
	{
		RenderCommand::Init();
		VulkanRenderer::Init();
	}

	void Renderer::Shudown()
	{
		VulkanRenderer::Shutdown();
	}

	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}
}
