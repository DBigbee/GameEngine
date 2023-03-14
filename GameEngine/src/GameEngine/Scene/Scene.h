#pragma once

#include <entt/entt.hpp>
#include "Transform.h"

namespace GE
{
	class Entity;

	class Scene
	{
	public:
		Scene();

		Entity Create(const std::string& name, const Transform& transform = {});

		void Update(float dellaTime);

		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry& Registry() { return m_Registry; }

	private:
		entt::registry m_Registry;


		friend class Entity;
	};
}