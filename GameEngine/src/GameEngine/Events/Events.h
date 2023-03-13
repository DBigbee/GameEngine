#pragma once

#include "pch.h"

namespace GE
{
	enum class EventType
	{
		None = 0,
		WindowResize,
		WindowClose
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
	virtual EventType GetEventType() const override { return GetStaticType();}


	class Event
	{
	public:

		virtual ~Event() = default;

		bool m_Handled = false;

		virtual EventType GetEventType() const = 0;
	};

	class EventDispatcher
	{
	public:

		EventDispatcher(Event& event)
			:m_Event(event) {}


		template<typename T, typename Func>
		bool Dispath(const Func& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled |= func(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};
}