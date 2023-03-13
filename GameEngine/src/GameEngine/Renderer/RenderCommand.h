#pragma once

#include "RendererAPI.h"

namespace GE
{
	class RenderCommand
	{
	public:

		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		static void SetViewport(uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(width, height);
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		static Ref<RendererAPI>& GetRendererAPI()
		{
			return s_RendererAPI;
		}

	private:

		static Ref<RendererAPI> s_RendererAPI;
	};
}