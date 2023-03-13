#include "pch.h"
#include "Entity.h"

namespace GE
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_Handle(handle), m_Scene(scene)
	{
	}
}
