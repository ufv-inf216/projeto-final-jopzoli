/*
* 
*/

#pragma once

#include "thirdparty.hpp"
#include <functional>
#include <map>

struct event_trigger
{
	enum Status
		: uint8_t
	{
		Failed,
		Passed,
	};

	const Status m_status{ Failed };

	template <typename _FnTy, typename... _Args>
	void then(_FnTy _fn, _Args&&... _args)
	{
		if (m_status == Passed)
			std::invoke(_fn, std::forward<_Args>(_args)...);
	}
};

struct keyboard_event_trigger
	: event_trigger
{
	enum Type
	{
		KeyDown = SDL_KEYDOWN,
		KeyUp = SDL_KEYUP,
	};

	static const uint8_t* keyState;

	static keyboard_event_trigger when(
		const SDL_KeyboardEvent& _e,
		Type _type,
		SDL_Keycode _keyCode,
		Uint32 _mod = KMOD_NONE,
		bool _repeat = false);
};

struct window_event_trigger
	: event_trigger
{
	static window_event_trigger when(
		const SDL_WindowEvent& _e,
		SDL_WindowEventID _type
	);
};
class EventFilter
{
public:
	enum EventType
		: Uint32
	{
		Quit = SDL_QUIT,
		AppTerminating = SDL_APP_TERMINATING,
		AppLowmemory = SDL_APP_LOWMEMORY,
		AppWillEnterBackground = SDL_APP_WILLENTERBACKGROUND,
		AppDidEnterBackground = SDL_APP_DIDENTERBACKGROUND,
		AppWillEnterForeground = SDL_APP_WILLENTERFOREGROUND,
		AppDidEnterForeground = SDL_APP_DIDENTERFOREGROUND,
		// ...
		DysplayEvent = SDL_DISPLAYEVENT,
		WindowEvent = SDL_WINDOWEVENT,
		SysWMEvent = SDL_SYSWMEVENT,
		KeyDown = SDL_KEYDOWN,
		KeyUp = SDL_KEYUP,
		TextEditing = SDL_TEXTEDITING,
		TextInput = SDL_TEXTINPUT,
		KeyMapChanged = SDL_KEYMAPCHANGED,
		MouseMotion = SDL_MOUSEMOTION,
		MouseButtonDown = SDL_MOUSEBUTTONDOWN,
		MouseButtonUp = SDL_MOUSEBUTTONUP,
		MouseWheel = SDL_MOUSEWHEEL,
		// ...
		AudioDeviceAdded = SDL_AUDIODEVICEADDED,
		AudioDeviceRemoved = SDL_AUDIODEVICEREMOVED,
		// ...
		UserEvent = SDL_USEREVENT,
	};

	EventFilter( );

	bool sendEvents( );

	void enableHandler(EventType _type);

	void disableHandler(EventType _type);

	void registerHandler(EventType _type, std::function<void(SDL_Event&)> _handler);

	void unregisterHandler(EventType _type) noexcept;

	void unregisterAllHandlers( ) noexcept;

private:
	std::pmr::multimap<EventType, std::pair<bool, std::function<void(SDL_Event&)>>> m_handlers;
};
