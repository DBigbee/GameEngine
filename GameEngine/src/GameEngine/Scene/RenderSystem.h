#pragma once

#include "System.h"

namespace GE
{
	class RenderSystem : public System
	{
	public:

		RenderSystem() = default;

		virtual void OnUpdate(class Scene* scene, float deltaTime);
	};
}