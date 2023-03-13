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

		void Update();

		void OnViewportResize(uint32_t width, uint32_t height);

	private:

		entt::registry m_Registry;


		friend class Entity;
	};
}