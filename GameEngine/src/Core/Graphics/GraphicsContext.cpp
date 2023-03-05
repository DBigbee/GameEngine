#include "pch.h"
#include "GraphicsContext.h"
#include "VulkanContext.h"

std::shared_ptr<GraphicsContext> GraphicsContext::Create(WinWindow* window)
{
	return std::make_shared<VulkanContext>(window);
}
