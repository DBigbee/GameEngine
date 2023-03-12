#include "pch.h"
#include "Components.h"

#include "Core/Graphics/Mesh/Model.h"


ModelComponent::ModelComponent()
{
}

ModelComponent::ModelComponent(Model* model)
{
	m_Model = std::shared_ptr<Model>(model);
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
