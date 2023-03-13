#include "pch.h"
#include "WinWindow.h"

#include "Renderer/GraphicsContext.h"
#include "Events/WindowEvents.h"
#include "Vulkan/VulkanRenderer.h"

namespace GE
{
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

	void WinWindow::OnUpdate()
	{
		glfwPollEvents();

		m_Context->SwapBuffers();
	}

	void WinWindow::SetEventCallback(const FWindowSpecification::EventCallbackFunc& callback)
	{
		m_Specification.EventCallback = callback;
	}

	void WinWindow::Init()
	{
		CreateNativeWindow();

		m_Context = GraphicsContext::Create(m_NativeWindow);

		m_Context->Init();
	}

	void WinWindow::CreateNativeWindow()
	{
		glfwInit();
		glfwSetErrorCallback(glfwErrorCallback);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		m_NativeWindow = glfwCreateWindow(m_Specification.m_Width, m_Specification.m_Height, m_Specification.m_Title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_NativeWindow, this);
		glfwSetWindowSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height)
			{
				auto winwindow = (WinWindow*)glfwGetWindowUserPointer(window);
				winwindow->m_Specification.m_Width = width;
				winwindow->m_Specification.m_Height = height;
				winwindow->m_FrameBufferResized = true;

				WindowResizeEvent event((uint32_t)width, (uint32_t)height);
				winwindow->m_Specification.EventCallback(event);

				//VulkanRenderer::OnWindowResized();
			});

		glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window) {
				
				auto winwindow = (WinWindow*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				winwindow->m_Specification.EventCallback(event);
			});
	}
}

