#pragma once

#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

#include "GraphicsContext.h"


class VulkanContext : public GraphicsContext
{
public:

	VulkanContext(WinWindow* window);

	~VulkanContext();

	void WaitIdle() override;

	void DrawFrame() override;

private:
	void CreateVertexBuffer();

private:

	std::unique_ptr<class Device> m_Device;

	std::unique_ptr<class Mesh> m_Triangle;

	std::unique_ptr<class Renderer> m_Renderer;

	std::unique_ptr<class SimpleRenderSystem> m_RenderSystem;
};