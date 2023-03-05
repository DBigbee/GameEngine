#pragma once

#include "Events/Events.h"
#include "Events/WindowEvents.h"


struct GLFWwindow;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class Application
{
public:

	Application();

	virtual ~Application();

	void Run();

	class WinWindow* GetWindow() { return m_Window.get(); }

	static Application* GetApplication() { return s_Instance; }

	void OnEvent(Event& event);

private:

	bool OnWindowResize(class WindowResizeEvent& event);

	void InitWindow();

	void MainLoop();

	void Terminate();

	std::unique_ptr<class WinWindow> m_Window;

	static Application* s_Instance;

	bool m_Minimized = false;
};

