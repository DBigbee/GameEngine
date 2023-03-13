#pragma once


#include <GLFW/glfw3.h>

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace GE
{
	class VulkanContext : public GraphicsContext
	{
	public:

		VulkanContext(GLFWwindow* windowHandle);

		~VulkanContext();

		void Init() override;

		void SwapBuffers() override;

	private:

		GLFWwindow* m_WindowHandle;
	};
}