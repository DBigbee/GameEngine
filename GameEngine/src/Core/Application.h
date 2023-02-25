#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Application
{
public:

	Application();

	virtual ~Application();

	void Run();

private:

	void InitWindow();

	void InitVulkan();

	void MainLoop();

	void Terminate();

	void CreateVKInstance();

	GLFWwindow* m_Window = nullptr;

	VkInstance m_Instance;
};

