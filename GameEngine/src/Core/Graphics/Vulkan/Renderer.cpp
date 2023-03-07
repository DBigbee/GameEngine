#include "pch.h"
#include "Renderer.h"
#include "Core/WinWindow.h"
#include "SwapChain.h"
#include "Core/WinWindow.h"
#include "Device.h"

Renderer::Renderer(Device* device, WinWindow* window)
	:m_Device(device), m_Window(window)
{
	RecreateSwapChain();
	CeateCommandBuffers();
}

Renderer::~Renderer()
{
	FreeCommandBuffers();
}

VkRenderPass Renderer::GetSwapChainRenderPass() const
{
	return m_SwapChain->GetRenderPass();
}

VkExtent2D Renderer::GetSwapChainExtents() const
{
	return m_SwapChain->GetExtents();
}

VkCommandBuffer Renderer::GetCurrentCommandBuffer() const
{
	assert(m_IsFrameStarted && "Cannot get command buffer when frame not in progress");
	return m_CommandBuffers[m_SwapChain->GetCurrentFrame()];
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

	auto commandBuffer = GetCurrentCommandBuffer();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording commandbuffer!");
	}


	return commandBuffer;
}

void Renderer::EndFrame()
{
	assert(m_IsFrameStarted && "Cant call end frame while already in progress!");


	auto commandBuffer = GetCurrentCommandBuffer();
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffers!");
	}

	//m_SwapChain->ResetFence();
	//vkResetCommandBuffer(commandBuffer, 0);


	auto result = m_SwapChain->SubmitCommandBuffers(m_CommandBuffers.data(), &m_CurrentFrameIndex);
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
	assert(commandBuffer == GetCurrentCommandBuffer()  && "Cant begin render on a different frame!");

	VkRenderPassBeginInfo renderpassInfo{};
	renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderpassInfo.renderPass = m_SwapChain->GetRenderPass();
	renderpassInfo.framebuffer = m_SwapChain->GetFrameBuffers()[m_CurrentFrameIndex];

	renderpassInfo.renderArea.offset = { 0 ,0 };
	renderpassInfo.renderArea.extent = m_SwapChain->GetExtents();

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderpassInfo.clearValueCount = 1;
	renderpassInfo.pClearValues = &clearColor;

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
	assert(commandBuffer == GetCurrentCommandBuffer() && "Cant end render pass on a different frame!");

	vkCmdEndRenderPass(commandBuffer);

}

void Renderer::CeateCommandBuffers()
{
	m_CommandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_Device->GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

	if (vkAllocateCommandBuffers(m_Device->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command buffers!");
	}
}

void Renderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
}

void Renderer::FreeCommandBuffers()
{
	vkFreeCommandBuffers(m_Device->GetDevice(), m_Device->GetCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	m_CommandBuffers.clear();
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
		if (m_SwapChain->GetImageCount() != m_CommandBuffers.size())
		{
			FreeCommandBuffers();
			CeateCommandBuffers();
		}
	}
}
