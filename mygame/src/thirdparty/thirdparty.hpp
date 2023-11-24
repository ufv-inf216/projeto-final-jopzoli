/*
*
*/

#pragma once

#include "utils/util.hpp"
#include <GL/glew.h>
#include <SDL.h>
// BUGBUG: hotfix, avoids getting error `Unresolved symbol SDL_main`
#undef main

#define SDL_SAFE_TERMINATE( )									\
auto sdl_safe_terminate = [ ]( )								\
{																\
	sdl_resources::terminateSubsystem(sdl_resources::Everything);\
};																\
FINALLY_DECL(sdl_safe_terminate);

struct opengl_resources
{
	static bool initializeGLEW( ) noexcept;
};

struct sdl_resources
{
	enum SubSystem
		: Uint32
	{
		Timer = SDL_INIT_TIMER,
		Audio = SDL_INIT_AUDIO,
		Video = SDL_INIT_VIDEO,
		Joystick = SDL_INIT_JOYSTICK,
		Haptic = SDL_INIT_HAPTIC,
		GameController = SDL_INIT_GAMECONTROLLER,
		Events = SDL_INIT_EVENTS,
		Everything = SDL_INIT_EVERYTHING,
	};

	static SubSystem initializeSubsystem(SubSystem _flags) noexcept;
	static void terminateSubsystem(SubSystem _flags) noexcept;
	static bool isInitialized(SubSystem _flags) noexcept;
};
OP_BITWISE_COMPAT_(sdl_resources::SubSystem);
