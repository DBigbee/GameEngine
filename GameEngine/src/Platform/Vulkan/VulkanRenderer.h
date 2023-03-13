#pragma once

#include "SimpleRenderSystem.h"
#include "Renderer/Camera.h"
#include "Scene/Components.h"

namespace GE
{
	class VulkanRenderer
	{
	public:

		VulkanRenderer();
		~VulkanRenderer();

		static void Init();
		static void BeginScene(const Camera* camera, const Transform& transform);
		static void EndScene();
		static void Shutdown();

		static void DrawModel(const glm::mat4 transform, ModelComponent& src);

		static void OnWindowResized();


	private:		
		static VkCommandBuffer BeginFrame();

		static void EndFrame();

		static void BeginRenderPass(VkCommandBuffer commandBuffer);

		static void EndRenderPass(VkCommandBuffer commandBuffer);

	private:

		static Ref<class CommandBuffer> s_CommandBuffer;
	};
}