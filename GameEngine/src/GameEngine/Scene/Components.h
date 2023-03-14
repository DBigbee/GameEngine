#pragma once

#include "Transform.h"
#include "Renderer/PerspectiveCamera.h"

namespace GE
{
	struct Component
	{
		Component() = default;
        Component(const Component&) = default;
	};

	struct TagComponent : public Component
	{
		TagComponent() = default;
		TagComponent(const std::string& name);
		TagComponent(const TagComponent&) = default;

		std::string m_Name = "Entity";
	};

	struct TransformComponent : public Component
	{
		TransformComponent() = default;
		TransformComponent(const Transform& transform);
		TransformComponent(const TransformComponent&) = default;

		Transform m_Transform;
	};

	struct CameraComponent : public Component
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		PerspectiveCamera m_PerspectiveCamera;
		
		bool m_IsPrimary = true;
	};

	struct ModelComponent : public Component
	{
		ModelComponent();
		ModelComponent(const Ref<class Model>& model);
		ModelComponent(const ModelComponent& other);

		Ref<class Model> m_Model;
	};
}