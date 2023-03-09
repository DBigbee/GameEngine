#include "pch.h"
#include "Renderer.h"
#include "Core/WinWindow.h"
#include "SwapChain.h"
#include "Core/WinWindow.h"
#include "Device.h"
#include "CommandBuffer.h"

Renderer::Renderer(Device* device, WinWindow* window)
	:m_Device(device), m_Window(window)
{
	RecreateSwapChain();
	CeateCommandBuffers();
}

Renderer::~Renderer()
{
	
}

VkRenderPass Renderer::GetSwapChainRenderPass() const
{
	return m_SwapChain->GetRenderPass();
}

VkExtent2D Renderer::GetSwapChainExtents() const
{
	return m_SwapChain->GetExtents();
}

uint32_t Renderer::GetSwapChainCurrentFrame() const
{
	return m_SwapChain->GetCurrentFrame();
}

VkCommandBuffer Renderer::BeginFrame()
{
	assert(!m_IsFrameStarted && "Cant call begin frame while already in progress!");

	VkResult result = m_SwapChain->AquireNextImage(&m_CurrentFrameIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return nullptr;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed tp acquire swap chain image!");
	}
	m_IsFrameStarted = true;

	auto commandBuffer = m_CommandBuffer->Begin(m_SwapChain->GetCurrentFrame());
	return commandBuffer;
}

void Renderer::EndFrame()
{
	assert(m_IsFrameStarted && "Cant call end frame while already in progress!");

	m_CommandBuffer->End();

	auto result = m_SwapChain->SubmitCommandBuffers(m_CommandBuffer->GetBuffers().data(), &m_CurrentFrameIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window->WasWindowResized())
	{
		m_Window->ResetWindowResizedFlag();
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	m_IsFrameStarted = false;
}

void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(m_IsFrameStarted && "Cant call begin swap render pass while already in progress!");
	assert(commandBuffer == m_CommandBuffer->GetCurrent() && "Cant begin render on a different frame!");

	VkRenderPassBeginInfo renderpassInfo{};
	renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderpassInfo.renderPass = m_SwapChain->GetRenderPass();
	renderpassInfo.framebuffer = m_SwapChain->GetFrameBuffers()[m_CurrentFrameIndex];

	renderpassInfo.renderArea.offset = { 0 ,0 };
	renderpassInfo.renderArea.extent = m_SwapChain->GetExtents();

	std::array<VkClearValue, 2> clearColors{};
	clearColors[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearColors[1].depthStencil = { 1.0f, 0 };
	renderpassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
	renderpassInfo.pClearValues = clearColors.data();

	vkCmdBeginRenderPass(commandBuffer, &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);

	auto extents = m_SwapChain->GetExtents();
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extents.width;
	viewport.height = (float)extents.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0 ,0 };
	scissor.extent = m_SwapChain->GetExtents();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(m_IsFrameStarted && "Cant call end swap render pass while already in progress!");
	assert(commandBuffer == m_CommandBuffer->GetCurrent() && "Cant end render pass on a different frame!");

	vkCmdEndRenderPass(commandBuffer);

}

void Renderer::CeateCommandBuffers()
{
	m_CommandBuffer = std::make_unique<CommandBuffer>(m_Device, SwapChain::MAX_FRAMES_IN_FLIGHT, 0);
}


void Renderer::RecreateSwapChain()
{
	auto extent = m_Window->GetExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = m_Window->GetExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(m_Device->GetDevice());

	if (m_SwapChain == nullptr)
	{
		m_SwapChain = std::make_unique<SwapChain>(m_Device, extent);
	}
	else
	{
		m_SwapChain = std::make_unique<SwapChain>(m_Device, extent, std::move(m_SwapChain));
		if (m_SwapChain->GetImageCount() != m_CommandBuffer->GetCount())
		{
			m_CommandBuffer->Free();
			CeateCommandBuffers();
		}
	}
}
