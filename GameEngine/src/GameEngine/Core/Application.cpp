#include "pch.h"
#include "Application.h"
#include "WinWindow.h"
#include "PlatformUtils.h"
#include "Renderer/Renderer.h"
#include "Vulkan/VulkanRenderer.h"

namespace GE
{

	Application::Application()
	{
		
		if (!s_Instance)
		{
			s_Instance = this;
		}

		InitWindow();

		Renderer::Init();
	}

	Application::~Application()
	{
		Renderer::Shudown();
	}

	void Application::Run()
	{
		
		while (m_Running)
		{
			float time = Time::GetTime();
			float deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{

				{
					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(deltaTime);
					}
				}

			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispath<WindowResizeEvent>(BIND_FUNCTION(Application::OnWindowResize));
		dispatcher.Dispath<WindowCloseEvent>(BIND_FUNCTION(Application::OnWindowClosed));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.m_Handled) break;
			(*it)->OnEvent(event);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.Push(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
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

	bool Application::OnWindowClosed(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	void Application::InitWindow()
	{
		FWindowSpecification specs;
		m_Window = MakeScope<WinWindow>(specs);
		m_Window->SetEventCallback(BIND_FUNCTION(Application::OnEvent));
	}

	Application* Application::s_Instance = nullptr;
}