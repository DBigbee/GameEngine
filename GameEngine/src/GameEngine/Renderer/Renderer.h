#pragma once

#include "RendererAPI.h"

namespace GE
{
	class Renderer
	{
	public:
		static void Init();
		static void Shudown();

		static void BeginScene();
		static void EndScene();

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}