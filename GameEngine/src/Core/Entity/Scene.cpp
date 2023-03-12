#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Core/Graphics/Vulkan/Renderer.h"
#include "Core/Graphics/Vulkan/Renderer.h"

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

void Scene::Update(class Renderer* renderer)
{
    auto view = m_Registry.view<TransformComponent, ModelComponent>();
    for (auto entity : view)
    {
        auto [transform, model] = view.get(entity);
       
    }
}
