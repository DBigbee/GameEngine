#include "pch.h"
#include "VulkanRendererAPI.h"
#include "Device.h"
#include "SwapChain.h"

namespace GE
{

	VulkanRendererAPI::~VulkanRendererAPI()
	{
		
	}

	void VulkanRendererAPI::Init()
	{
		s_Device = MakeRef<Device>();

		CreateCommandBuffer();
	}

	void VulkanRendererAPI::SetViewport(uint32_t width, uint32_t height)
	{
		
	}

	void VulkanRendererAPI::SetClearColor(const glm::vec4& color)
	{
		
	}

	void VulkanRendererAPI::Clear()
	{

	}

	void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		if (!vertexArray->GetIndexBuffer())
		{
			vkCmdDraw(s_CommandBuffer->GetCurrent(), count, 1, 0, 0);
		}
		else
		{
			vkCmdDrawIndexed(s_CommandBuffer->GetCurrent(), count, 1, 0, 0, 0);
		}
	}

	Ref<CommandBuffer> VulkanRendererAPI::CreateCommandBuffer()
	{
		return s_CommandBuffer = MakeRef<CommandBuffer>(SwapChain::MAX_FRAMES_IN_FLIGHT);
	}
}
