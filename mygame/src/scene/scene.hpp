/*
*
*/

#pragma once

#include "actor/actor.hpp"
#include "object/object.hpp"
#include "thirdparty/eventfilter.hpp"
#include "thirdparty/renderer.hpp"
#include <deque>
#include <map>
#include <vector>

class Scene
	: public Object
{
public:
	OBJIMPL( );

	enum Status
	{
		Active,
		Inactive
	};

	Scene(class Game* _game);

	NODISCARD Status status( ) const noexcept
	{
		return m_status;
	}
	
	void play( ) noexcept;

	void pause( ) noexcept;

	virtual void onPlay( ) noexcept
	{ }

	virtual void onPause( ) noexcept
	{ }

	virtual void onAudioDeviceEvent(SDL_AudioDeviceEvent& _e)
	{ }

	virtual void onKeyboardEvent(SDL_KeyboardEvent& _e)
	{ }

	virtual void onMouseButtonEvent(SDL_MouseButtonEvent& _e)
	{ }

	virtual void onMouseMotionEvent(SDL_MouseMotionEvent& _e)
	{ }

	virtual void onMouseWheelEvent(SDL_MouseWheelEvent& _e)
	{ }

	virtual void onQuitEvent(SDL_QuitEvent& _e)
	{ }

	virtual void onSysWMEvent(SDL_SysWMEvent& _e)
	{ }

	virtual void onWindowEvent(SDL_WindowEvent& _e)
	{ }

	void tick(float _deltaTime);

protected:
	std::pmr::map<int, object_ptr<Actor>> actors;
	Game* game;

	virtual void tickScene(float _deltaTime)
	{ }

private:
	Status m_status;

	void _tickActors(float _deltaTime);
};
OBJDECL(Scene, Object);
