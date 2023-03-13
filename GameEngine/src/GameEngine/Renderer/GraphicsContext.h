#pragma once

struct GLFWwindow;

namespace GE
{

	class GraphicsContext
	{
	public:

		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Ref<GraphicsContext> Create(GLFWwindow* window);
	};
}