#pragma once

#include "Transform.h"
#include "Renderer/PerspectiveCamera.h"

namespace GE
{
	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const std::string& name);
		TagComponent(const TagComponent&) = default;

		std::string m_Name = "Entity";
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const Transform& transform);
		TransformComponent(const TransformComponent&) = default;

		Transform m_Transform;
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		PerspectiveCamera m_PerspectiveCamera;
	};

	struct ModelComponent
	{
		ModelComponent();
		ModelComponent(const Ref<class Model>& model);
		ModelComponent(const ModelComponent& other);

		Ref<class Model> m_Model;
	};
}