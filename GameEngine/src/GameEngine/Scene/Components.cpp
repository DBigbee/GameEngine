#include "pch.h"
#include "Components.h"

#include "Renderer/Model.h"

namespace GE
{
	ModelComponent::ModelComponent()
	{
	}

	ModelComponent::ModelComponent(const Ref<Model>& model)
	{
		m_Model = Ref<Model>(model);
	}

	ModelComponent::ModelComponent(const ModelComponent& other)
	{
		m_Model = other.m_Model;
	}

	TransformComponent::TransformComponent(const Transform& transform)
		:m_Transform(transform)
	{

	}

	TagComponent::TagComponent(const std::string& name)
		:m_Name(name)
	{
	}
}
