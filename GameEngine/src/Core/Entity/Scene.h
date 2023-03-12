#pragma once

#include <entt/entt.hpp>
#include "Transform.h"

class Entity;

class Scene
{
public:

	Scene();

	Entity Create(const std::string& name, const Transform& transform = {});

	void Update(class Renderer* renderer);

private:

	entt::registry m_Registry;


	friend class Entity;
};