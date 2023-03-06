#include "pch.h"
#include "VulkanContext.h"
#include "Core/Application.h"
#include "Mesh/Mesh.h"
#include "Core/Graphics/Vulkan/Renderer.h"
#include "Core/Graphics/Vulkan/SimpleRenderSystem.h"
#include "Core/Graphics/Vulkan/Device.h"

void sierpinski(std::vector<Vertex>& vertices, int depth, glm::vec3 left, glm::vec3 right, glm::vec3 top)
{
	if (depth <= 0)
	{
		vertices.push_back({ top,  {0.0f, 0.0f, 1.0f} });
		vertices.push_back({ right, {0.0f, 0.0f, 1.0f} });
		vertices.push_back({ left, {0.0f, 0.0f, 1.0f} });
	}
	else
	{
		auto leftTop = 0.5f * (left + top);
		auto rightTop = 0.5f * (right + top);
		auto leftRight = 0.5f * (left + right);
		sierpinski(vertices, depth - 1, left, leftRight, leftTop);
		sierpinski(vertices, depth - 1, leftRight, right, rightTop);
		sierpinski(vertices, depth - 1, leftTop, rightTop, top);
	}
}

VulkanContext::VulkanContext(WinWindow* window)
	:GraphicsContext(window)
{	
	m_Device = std::make_unique<Device>(window);
	
	m_Renderer = std::make_unique<Renderer>(m_Device.get(), window);

	m_RenderSystem = std::make_unique<SimpleRenderSystem>(m_Device.get(), m_Renderer->GetSwapChainRenderPass());
	
	CreateVertexBuffer();
}

VulkanContext::~VulkanContext()
{
	

}

void VulkanContext::WaitIdle()
{
	vkDeviceWaitIdle(m_Device->GetDevice());
}

void VulkanContext::DrawFrame()
{
	if (auto commandBuffer = m_Renderer->BeginFrame())
	{
		m_Renderer->BeginSwapChainRenderPass(commandBuffer);

		//begin offscreen shadow pass
		// render casting objects
		// end offscreen shadow pass
		
		//Draw objects
		
		auto extents = m_Renderer->GetSwapChainExtents();
		m_RenderSystem->RenderObjects(commandBuffer, m_Renderer->GetSwapChainCurrentFrame(), extents.width , extents.height , { m_Triangle.get() });

		m_Renderer->EndSwapChainRenderPass(commandBuffer);
		m_Renderer->EndFrame();
	}
}

void VulkanContext::CreateVertexBuffer()
{
	std::vector<Vertex> vertices =
	{
		{{-0.5f, -.5f, 0.0f},{1.0f, 0.0f, 0.0f}},
		{{0.5f, -.5f, 0.0f},{1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f},{0.0f, 1.0f, 0.0f}},
		{{-.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f}}
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 2, 2, 3, 0
	};
	
	m_Triangle = std::make_unique<Mesh>(m_Device.get(), vertices, indices);
}