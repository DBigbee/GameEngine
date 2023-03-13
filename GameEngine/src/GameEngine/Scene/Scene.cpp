#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Vulkan/VulkanRenderer.h"

namespace GE
{
    Scene::Scene()
    {

    }

    Entity Scene::Create(const std::string& name, const Transform& transform)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
        entity.AddComponent<TransformComponent>(transform);
        return entity;
    }

    void Scene::Update()
    {
        Camera* mainCamera = nullptr;
        Transform cameraTransform;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get(entity);

                mainCamera = &camera.m_PerspectiveCamera;
                cameraTransform = transform.m_Transform;
            }
        }
      
        if (mainCamera)
        {
            VulkanRenderer::BeginScene(mainCamera, cameraTransform);

            auto view = m_Registry.view<TransformComponent, ModelComponent>();
            for (auto entity : view)
            {
                auto [transform, model] = view.get(entity);

                VulkanRenderer::DrawModel(transform.m_Transform.ToMat4(), model);
            }

            VulkanRenderer::EndScene();
        }
        
    }
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {  
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& camera = view.get<CameraComponent>(entity);

            camera.m_PerspectiveCamera.SetViewportSize(width, height);
        }
    }
}