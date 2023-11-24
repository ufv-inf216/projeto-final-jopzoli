/*
* 
*/

#pragma once

#include "thirdparty/renderer.hpp"
#include "thirdparty/eventfilter.hpp"
#include "actor/camera/camera.hpp"

class Game
{
public:
	enum RuntimeStatus
	{
		Running,
		Paused,
		Halted,
		Quit
	};

	Game( );
	~Game( );

	NODISCARD RuntimeStatus state( ) noexcept;

	NODISCARD Renderer& renderer( ) noexcept;

	NODISCARD EventFilter& eventFilter( ) noexcept;

	void run( );

private:
	RuntimeStatus status;
	Camera camera;
	//Scene m_scene; // TODO: stack
	EventFilter m_eventFilter;
	Renderer m_renderer;

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
