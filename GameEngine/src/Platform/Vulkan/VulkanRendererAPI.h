#pragma once

#include "Renderer/RendererAPI.h"
#include "Device.h"
#include "CommandBuffer.h"

namespace GE
{
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		~VulkanRendererAPI();

		virtual void Init() override;

		virtual void SetViewport(uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

		Ref<Device>& GetDevice() { return s_Device; }

		Ref<CommandBuffer>& GetCommandBuffer() { return s_CommandBuffer; }

		Ref<CommandBuffer> CreateCommandBuffer();

	private:
		Ref<Device> s_Device;

		Ref<CommandBuffer> s_CommandBuffer;
	};
}
