#include "pch.h"
#include "SceneLayer.h"
#include "Vulkan/VulkanRenderer.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"

namespace GE
{
	SceneLayer::SceneLayer()
		:Layer("Scene Layer")
	{
		std::vector<Vertex> vertices =
		{
			{{-0.5f, -.5f, 0.0f},	{1.0f, 0.0f, 0.0f}	,{1.0f, 0.0f}},
			{{0.5f, -.5f, 0.0f},	{1.0f, 1.0f, 1.0f}	,{0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f},	{0.0f, 1.0f, 0.0f}	,{0.0f, 1.0f}},
			{{-.5f, 0.5f, 0.0f},	{0.0f, 0.0f, 1.0f}	,{1.0f, 1.0f}},

			{{-0.5f, -.5f, -.5f},	{1.0f, 0.0f, 0.0f}	,{1.0f, 0.0f}},
			{{0.5f, -.5f, -.5f},	{1.0f, 1.0f, 1.0f}	,{0.0f, 0.0f}},
			{{0.5f, 0.5f, -.5f},	{0.0f, 1.0f, 0.0f}	,{0.0f, 1.0f}},
			{{-.5f, 0.5f, -.5f},	{0.0f, 0.0f, 1.0f}	,{1.0f, 1.0f}}
		};

		std::vector<uint32_t> indices =
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		m_Cube = MakeRef<Model>("assets/models/cube.obj");
		m_Plane = MakeRef<Mesh>(vertices, indices);
		m_Cube->AddMesh(m_Plane);
	}

	void SceneLayer::OnAttach()
	{
		

		m_Scene = MakeScope<Scene>();
		auto camera = m_Scene->Create("Camera");
		camera.AddComponent<CameraComponent>();
		auto& tComp = camera.GetComponent<TransformComponent>();
		tComp.m_Transform.m_Position = glm::vec3(2.0f , 2.0f, 2.0f);

		auto c = m_Scene->Create("Cube");
		c.AddComponent<ModelComponent>(m_Cube);
		c.GetComponent<TransformComponent>().m_Transform.m_Scale = glm::vec3(1.f);
	}

	void SceneLayer::OnUpdate(float deltaTime)
	{
		m_Scene->Update();
	}

	void SceneLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispath<WindowResizeEvent>(BIND_FUNCTION(SceneLayer::OnWindowResized));
	}

	bool SceneLayer::OnWindowResized(WindowResizeEvent& event)
	{
		if (m_Scene)
		{
			m_Scene->OnViewportResize(event.GetWidth(), event.GetHeight());
		}
		return false;
	}
}