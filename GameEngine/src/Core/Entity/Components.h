#pragma once

#include "Transform.h"

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

struct ModelComponent
{
	ModelComponent();
	ModelComponent(class Model* model);
	ModelComponent(const ModelComponent& other);

	std::shared_ptr<class Model> m_Model;
};