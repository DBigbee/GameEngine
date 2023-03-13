#pragma once

#include "Events/Events.h"
#include "Events/WindowEvents.h"
#include "LayerStack.h"

namespace GE
{
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;

	class Application
	{
	public:

		Application();

		virtual ~Application();

		void Run();

		class WinWindow& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:

		bool OnWindowResize(class WindowResizeEvent& event);

		bool OnWindowClosed(class WindowCloseEvent& event);

		void InitWindow();

		Scope<class WinWindow> m_Window;

		static Application* s_Instance;

		bool m_Minimized = false;

		bool m_Running = true;

		float m_LastFrameTime = 0.0f;

		LayerStack m_LayerStack;
	};
}
