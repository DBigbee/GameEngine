#include "pch.h"
#include "Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



Application::Application()
{
}

Application::~Application()
{
}

void Application::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Terminate();
}

void Application::InitWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void Application::InitVulkan()
{
	CreateVKInstance();
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();
	}
}



void Application::Terminate()
{
	glfwDestroyWindow(m_Window);

	glfwTerminate();
}

void Application::CreateVKInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
}
