#include "eventfilter.hpp"
#include "memory/memory.hpp"

EventFilter::EventFilter( )
	: m_handlers{ singleton_memory::getPool( ) }
{ }

bool EventFilter::sendEvents( )
{
	SDL_Event e;
	EventType type;
	while (SDL_PollEvent(&e))
	{
		type = EventType{ e.type };
		auto it = m_handlers.find(type);
		if (it != m_handlers.end( ))
		{
			if (it->second.first)
				std::invoke(it->second.second, e);
		}
	}

	return true;
}

bool EventFilter::registerHandler(EventType _type, std::function<void(SDL_Event&)> _handler)
{
	auto it = m_handlers.find(_type);
	if (it != m_handlers.end( ))
	{
		it->second = std::make_pair(true, _handler);
		return true;
	}
	m_handlers.emplace(_type, std::make_pair(true, _handler));

	return false;
}

inline void EventFilter::unregisterHandler(EventType _type) noexcept
{
	m_handlers.erase(_type);
}

inline void EventFilter::unregisterAllHandlers( ) noexcept
{
	m_handlers.clear( );
}

bool EventFilter::omitEvent(EventType _type)
{
	auto it = m_handlers.find(_type);
	if (it != m_handlers.end( ))
	{
		it->second.first = false;
		return true;
	}

	return false;
}

keyboard_event_trigger keyboard_event_trigger::whenKeyDown(
	const SDL_KeyboardEvent& _e,
	SDL_Keycode _keyCode,
	Uint32 _mod,
	bool _repeat)
{
	// TODO: check remap
	return {
		Status{ (_e.type == SDL_KEYDOWN)
		&& (_e.keysym.sym == _keyCode)
		&& _mod == KMOD_NONE || (_e.keysym.mod & _mod)
		&& (_repeat == (_e.repeat != 0)) }
	};
}

keyboard_event_trigger keyboard_event_trigger::whenKeyUp(
	const SDL_KeyboardEvent& _e,
	SDL_Keycode _keyCode,
	Uint32 _mod,
	bool _repeat)
{
	// TODO: check remap
	return {
		Status{ (_e.type == SDL_KEYUP)
		&& (_e.keysym.sym == _keyCode)
		&& _mod == KMOD_NONE || (_e.keysym.mod & _mod)
		&& (_repeat == (_e.repeat != 0)) }
	};
}

window_event_trigger window_event_trigger::when(const SDL_WindowEvent& _e, SDL_WindowEventID _type)
{
	return { Status{ _e.event == _type } };
}
