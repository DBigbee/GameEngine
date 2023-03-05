#include "pch.h"
#include "Application.h"
#include "WinWindow.h"
#include "Graphics/GraphicsContext.h"


Application::Application()
{
	
	if (!s_Instance)
	{
		s_Instance = this;
	}
}

Application::~Application()
{
	s_Instance = nullptr;
}

void Application::Run()
{
	InitWindow();
	MainLoop();
	Terminate();
}

void Application::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispath<WindowResizeEvent>(BIND_FUNCTION(Application::OnWindowResize));

}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
	if (event.GetWidth() == 0 || event.GetHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;

	return false;
}

void Application::InitWindow()
{
	FWindowSpecification specs;
	m_Window = std::make_unique<WinWindow>(specs);
	m_Window->SetEventCallback(BIND_FUNCTION(Application::OnEvent));
}


void Application::MainLoop()
{
	if (m_Window)
	{
		m_Window->Update();
	}
}


void Application::Terminate()
{
	
}


Application* Application::s_Instance = nullptr;