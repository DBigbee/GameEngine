#include "pch.h"
#include "VulkanContext.h"
#include "Core/Application.h"
#include "Mesh/Mesh.h"
#include "Core/Graphics/Vulkan/Renderer.h"
#include "Core/Graphics/Vulkan/SimpleRenderSystem.h"
#include "Core/Graphics/Vulkan/Device.h"
#include "Core/Graphics/Vulkan/UniformBuffer.h"
#include "Core/Graphics/Vulkan/SwapChain.h"
#include "Core/Graphics/Vulkan/Descriptors.h"
#include "Core/Graphics/Vulkan/GraphicsPipeline.h"
#include "Core/Graphics/Vulkan/UniformBufferObject.h"
#include "Core/Graphics/Vulkan/Texture2D.h"

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

	CreateUniformBuffers();

	m_DescriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	m_DescriptorPool = DescriptorPool::Builder(m_Device.get())
		.SetMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
		.Build();

	m_DescriptorSetLayout = DescriptorSetLayout::Builder(m_Device.get())
		.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
		.Build();

	for (int i = 0; i < m_DescriptorSets.size(); i++)
	{
		auto bufferInfo = m_UniformBuffers[i]->GetDescriptorBufferInfo();
		DescriptorWriter(m_DescriptorSetLayout.get(), m_DescriptorPool.get())
			.WriteBuffer(0, &bufferInfo)
			.Build(m_DescriptorSets[i]);
	}


	m_RenderSystem = std::make_unique<SimpleRenderSystem>(m_Device.get(), m_Renderer->GetSwapChainRenderPass(), 
		m_DescriptorSetLayout->GetSetLayout());
	
	
	CreateVertexBuffer();

	//m_Texture = std::make_unique<Texture2D>(m_Device.get(), "assets/textures/statue-g2725c3c85_1920.jpg");

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
		//m_Texture->CreatePipelineImageBarrier(commandBuffer, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		
		//begin offscreen shadow pass
		// render casting objects
		// end offscreen shadow pass

		//update ubo
		auto currentFrame = m_Renderer->GetSwapChainCurrentFrame();
		UpdateUniformBuffer(currentFrame);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_RenderSystem->GetPipelineLayout(),
			0, 1, &m_DescriptorSets[currentFrame], 0, nullptr);

		
		//Draw objects
		
		auto extents = m_Renderer->GetSwapChainExtents();
		m_RenderSystem->RenderObjects(commandBuffer, { m_Triangle.get() });

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

void VulkanContext::CreateUniformBuffers()
{
	m_UniformBuffers.resize((size_t)SwapChain::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++)
	{
		m_UniformBuffers[i] = std::make_unique<UniformBuffer>(m_Device.get(), sizeof(FUniformBufferObject), 1);
	}
}


void VulkanContext::UpdateUniformBuffer(uint32_t currentImage)
{
	auto extent = m_Renderer->GetSwapChainExtents();
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	FUniformBufferObject ubo{};
	ubo.m_Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.m_View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.m_Projection = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);

	//invert for vulkan
	ubo.m_Projection[1][1] *= -1;

	m_UniformBuffers[currentImage]->WriteToBuffer(&ubo);
	//m_UniformBuffers[currentImage]->Flush();
}

