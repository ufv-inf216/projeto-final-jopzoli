#include "eventfilter.hpp"
#include "utils/logger.hpp"
#include "memory/memory.hpp"

const uint8_t* keyboard_event_trigger::keyState = SDL_GetKeyboardState(nullptr);

keyboard_event_trigger keyboard_event_trigger::when(
	const SDL_KeyboardEvent& _e,
	Type _type,
	SDL_Keycode _keyCode,
	Uint32 _mod,
	bool _repeat)
{
	if (_e.type == KeyDown)
	{
		return {
			_e.keysym.sym == _keyCode
			&& _mod == KMOD_NONE || _e.keysym.mod & _mod
			&& (!_repeat || _e.repeat)
			? Passed : Failed
		};
	} else
	{
		return {
			_e.keysym.sym == _keyCode
			&& _mod == KMOD_NONE || _e.keysym.mod & _mod
			&& (!_repeat || _e.repeat)
			? Passed : Failed
		};
	}
}

window_event_trigger window_event_trigger::when(
	const SDL_WindowEvent& _e,
	SDL_WindowEventID _type)
{
	return { Status{ _e.event == _type } };
}

EventFilter::EventFilter( )
	: m_handlers{ singleton_memory::getAssynchronousPool( ) }
{ }

bool EventFilter::sendEvents( )
{
	SDL_Event e;
	EventType type;
	while (SDL_PollEvent(&e))
	{
		SDL_PumpEvents( );
		type = EventType{ e.type };
		auto it = m_handlers.find(type);
		if (it != m_handlers.end( ))
		{
			if (it->second.first)
				std::invoke(it->second.second, e);
		}
	}
	keyboard_event_trigger::keyState = SDL_GetKeyboardState(nullptr);

	return true;
}

void EventFilter::enableHandler(EventType _type)
{
	for (auto [it, rangeEnd] = m_handlers.equal_range(_type); it != rangeEnd; it++)
		it->second.first = true;
}

void EventFilter::disableHandler(EventType _type)
{
	for (auto [it, rangeEnd] = m_handlers.equal_range(_type); it != rangeEnd; it++)
		it->second.first = false;
}

void EventFilter::registerHandler(
	EventType _type,
	std::function<void(SDL_Event&)> _handler)
{
	m_handlers.emplace(_type, std::make_pair(true, _handler));
}

void EventFilter::unregisterHandler(EventType _type) noexcept
{
	m_handlers.erase(_type);
}

void EventFilter::unregisterAllHandlers( ) noexcept
{
	m_handlers.clear( );
}
