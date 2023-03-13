#include "pch.h"
#include "RenderCommand.h"

namespace GE
{
	Ref<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}