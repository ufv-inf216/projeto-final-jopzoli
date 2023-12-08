#include "thirdparty.hpp"

bool opengl_resources::initializeGLEW( ) noexcept
{
	static bool m_status = false;
	if (!m_status)
	{
		m_status = glewInit( ) == GLEW_OK;
		glGetError( );
	}

	return m_status;
}

sdl_resources::SubSystem sdl_resources::initializeSubsystem(SubSystem _flags) noexcept
{
#define SDL_INITIALIZER_DEF_CODE -1
#define SDL_INITIALIZER_INIT_SUBSYSTEM(x)			\
	if (_flags & x)									\
	{												\
		static int _##x = SDL_INITIALIZER_DEF_CODE;	\
		if (_##x < 0)								\
		{											\
			_##x = SDL_InitSubSystem(x);			\
			pass |= _##x == 0 ? x : 0;				\
		}											\
	}

	int pass = 0;
	if (_flags & Everything)
		pass = SDL_InitSubSystem(Everything) == 0 ? Everything : 0;
	else
	{
		SDL_INITIALIZER_INIT_SUBSYSTEM(Timer);
		SDL_INITIALIZER_INIT_SUBSYSTEM(Audio);
		SDL_INITIALIZER_INIT_SUBSYSTEM(Video);
		SDL_INITIALIZER_INIT_SUBSYSTEM(Joystick);
		SDL_INITIALIZER_INIT_SUBSYSTEM(Haptic);
		SDL_INITIALIZER_INIT_SUBSYSTEM(GameController);
		SDL_INITIALIZER_INIT_SUBSYSTEM(Events);
	}

	return static_cast<SubSystem>(pass);
}

void sdl_resources::terminateSubsystem(SubSystem _flags) noexcept
{
	SDL_QuitSubSystem(_flags);

	if (SDL_WasInit(0) == 0)
		SDL_Quit( );
}

bool sdl_resources::isInitialized(SubSystem _flags) noexcept
{
	return (SDL_WasInit(_flags) ^ _flags) == 0;
}
