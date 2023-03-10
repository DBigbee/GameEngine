#pragma once

#include <GameEngine.h>

namespace GE
{
	class SceneLayer : public Layer
	{
	public:

		SceneLayer();

		void OnAttach() override;

		void OnUpdate(float deltaTime) override;

		void OnEvent(Event& event) override;

		bool OnWindowResized(WindowResizeEvent& event);

		Scope<Scene> m_Scene;

		Ref<Model> m_Cube;

		Ref<Model> m_Plane;

		Ref<Mesh> m_PlaneMesh;
	};
}