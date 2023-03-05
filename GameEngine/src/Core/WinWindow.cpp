#include "pch.h"
#include "WinWindow.h"

#include "Graphics/GraphicsContext.h"
#include "Core/Events/WindowEvents.h"



void glfwErrorCallback(int code, const char* description)
{
	std::cerr << "GLFW Error " << code << ": " << description << std::endl;
}


WinWindow::WinWindow(const FWindowSpecification& Specification)
{
	m_Specification = Specification;

	Init();
}

WinWindow::~WinWindow()
{
	Shutdown();
}

void WinWindow::Shutdown()
{
	
	glfwDestroyWindow(m_NativeWindow);

	glfwTerminate();
}

void WinWindow::Update()
{
	while (!ShouldClose())
	{
		glfwPollEvents();

		m_Context->DrawFrame();
	}

	m_Context->WaitIdle();
}

void WinWindow::SetEventCallback(const FWindowSpecification::EventCallbackFunc& callback)
{
	m_Specification.EventCallback = callback;
}

void WinWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(m_NativeWindow);
	createInfo.hinstance = GetModuleHandle(nullptr);

	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface!");
	}
}

void WinWindow::Init()
{
	CreateNativeWindow();
	
}

void WinWindow::CreateNativeWindow()
{
	glfwInit();
	glfwSetErrorCallback(glfwErrorCallback);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	m_NativeWindow = glfwCreateWindow(m_Specification.m_Width, m_Specification.m_Height, m_Specification.m_Title.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_NativeWindow, this);
	glfwSetFramebufferSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height)
		{
			auto winwindow = (WinWindow*)glfwGetWindowUserPointer(window);
			winwindow->m_Specification.m_Width = width;
			winwindow->m_Specification.m_Height = height;
			winwindow->m_FrameBufferResized = true;

			WindowResizeEvent event((uint32_t)width, (uint32_t)height);
			winwindow->m_Specification.EventCallback(event);
		});

	m_Context = GraphicsContext::Create(this);
}

