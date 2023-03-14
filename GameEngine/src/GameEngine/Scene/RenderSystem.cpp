#include "pch.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "Vulkan/VulkanRenderer.h"
#include "Renderer/Camera.h"
#include "Components.h"

namespace GE
{
	void RenderSystem::OnUpdate(Scene* scene, float deltaTime)
	{
		Camera* mainCamera = nullptr;
		Transform cameraTransform;
		{
			auto view = scene->Registry().view<TransformComponent, CameraComponent>();
			for (auto entity : view)
				{
					auto [transform, camera] = view.get(entity);

					if (camera.m_IsPrimary)
						{
							mainCamera = &camera.m_PerspectiveCamera;
							cameraTransform = transform.m_Transform;
						}
				}
		}

		if (mainCamera)
			{
				VulkanRenderer::BeginScene(mainCamera, cameraTransform);

				auto view = scene->Registry().view<TransformComponent, ModelComponent>();
				for (auto entity : view)
					{
						auto [transform, model] = view.get(entity);

						VulkanRenderer::DrawModel(transform.m_Transform.ToMat4(), model);
					}

				VulkanRenderer::EndScene();
			}
	}
}

