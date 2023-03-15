#pragma once

#include <GameEngine/Core/Application.h>
#include <GameEngine/Core/EntryPoint.h>

#include "SceneLayer.h"


namespace GE
{
	class GameProject : public Application
	{
	public:

		GameProject()
		{
			PushLayer(new SceneLayer());
		}
	};

	Application* CreateApplication()
	{
		return new GameProject();
	}
}