#include "pch.h"
#include "PlatformUtils.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace GE
{
	float Time::GetTime()
	{
		return (float)glfwGetTime();
	}
}