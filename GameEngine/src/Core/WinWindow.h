#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

struct GLFWwindow;
class Event;

struct FWindowSpecification
{
	using EventCallbackFunc = std::function<void(Event&)>;

	uint32_t m_Width = 800;

	uint32_t m_Height = 600;

	std::string m_Title = "New Window";

	EventCallbackFunc EventCallback;
};

class WinWindow
{
public:
	
	WinWindow(const FWindowSpecification& Specification = FWindowSpecification());

	virtual ~WinWindow();

	GLFWwindow* GetNativeWindow() const { return m_NativeWindow; }

	const FWindowSpecification& GetSpecification() const { return m_Specification; }

	void Shutdown();

	void Update();

	bool ShouldClose() { return glfwWindowShouldClose(m_NativeWindow); }

	VkExtent2D GetExtent() { return { (uint32_t)m_Specification.m_Width, (uint32_t)m_Specification.m_Height }; }

	void SetEventCallback(const FWindowSpecification::EventCallbackFunc& callback);

	class GraphicsContext* GetGraphicsContext() { return m_Context.get(); }

	void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	bool WasWindowResized() const { return m_FrameBufferResized; }

	void ResetWindowResizedFlag() { m_FrameBufferResized = false; }

private:

	void Init();

	void CreateNativeWindow();

private:

	std::shared_ptr<class GraphicsContext> m_Context;

	GLFWwindow* m_NativeWindow;

public:
	FWindowSpecification m_Specification;

	bool m_FrameBufferResized = false;
};

