/*
* 
*/

#pragma once

#include "thirdparty/renderer.hpp"
#include "thirdparty/eventfilter.hpp"
#include "scene/scene.hpp"

class Game
{
public:
	enum RuntimeStatus
	{
		Running,
		Inactive,
		Halted,
		Quit
	};

	Game( );
	~Game( );

	NODISCARD RuntimeStatus state( ) noexcept;

	NODISCARD Renderer& renderer( ) noexcept;

	NODISCARD EventFilter& eventFilter( ) noexcept;

	void quit( ) noexcept;

	void run( );

private:
	object_ptr<Scene> m_currentScene;
	EventFilter m_eventFilter;
	Renderer m_renderer;
	uint32_t m_ticksPassed;
	RuntimeStatus m_status;

	void _registerEventHandlers( );
	void _onAudioDeviceEvent(SDL_AudioDeviceEvent& _e);
	void _onKeyboardEvent(SDL_KeyboardEvent& _e);
	void _onMouseButtonEvent(SDL_MouseButtonEvent& _e);
	void _onMouseMotionEvent(SDL_MouseMotionEvent& _e);
	void _onMouseWheelEvent(SDL_MouseWheelEvent& _e);
	void _onQuitEvent(SDL_QuitEvent& _e);
	void _onSysWMEvent(SDL_SysWMEvent& _e);
	void _onWindowEvent(SDL_WindowEvent& _e);
};
