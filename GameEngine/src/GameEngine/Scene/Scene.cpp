#include "pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "System.h"
#include "Vulkan/VulkanRenderer.h"


namespace GE
{
	Scene::Scene()
	{
		SystemRegistry::RegisterSystem<RenderSystem>();
	}

	Entity Scene::Create(const std::string& name, const Transform& transform)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		entity.AddComponent<TransformComponent>(transform);
		return entity;
	}

	void Scene::Update(float deltaTime)
	{
		auto systems = SystemRegistry::GetRegistry();
		for (auto& system : systems)
		{
			system.second->OnUpdate(this, deltaTime);
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