#pragma once

#include "Core/Core.h"
#include "Core/TypeInfo.h"
#include <unordered_map>
#include <type_traits>

namespace GE
{
	class System;

	class SystemRegistry
	{
	private:
		SystemRegistry() = delete;

	public:
		using RegisteredSystems = std::unordered_map<type_info, Ref<System>>;

		template <typename Type>
		static void RegisterSystem()
		{
			if (!std::is_base_of_v<System, Type>)
			{
				throw std::runtime_error("Type is derived from System");
			}
			const auto type = type_id<Type>();
			if (!m_SystemRegistry.contains(type))
			{
				m_SystemRegistry.emplace(type_id<Type>(),
					MakeRef<Type>());
			}
		}

		static RegisteredSystems GetRegistry()
		{
			return m_SystemRegistry;
		}


	private:
		static RegisteredSystems m_SystemRegistry;
	};

	class System
	{
	public:
		System() = default;
		virtual ~System() = default;

		virtual void OnUpdate(class Scene* scene, float deltaTime){};
	};
}