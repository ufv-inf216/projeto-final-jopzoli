#include "memory/monotonicstorage.hpp"
#include "renderer.hpp"
#include "utils/logger.hpp"

Renderer::Renderer( ) noexcept
	: m_glContext{ },
	m_window{ }
{ }

Renderer::~Renderer( )
{
	_EXPECTS(!m_window, "terminate has not been called");
}

bool Renderer::initialize(
	const std::string_view& _title,
	int _x,
	int _y,
	int _width,
	int _height,
	Flags _flags)
{
	_EXPECTS(!m_window, "terminate has not been called to clear out attached window");
	std::string log;
	sdl_resources::initializeSubsystem(sdl_resources::Video);
	if (!sdl_resources::isInitialized(sdl_resources::Video))
	{
		log = "could not initialize SDL resource Video, SDL got message: ";
		log += SDL_GetError( );
		Logger::w(log);

		return false;
	}

	if (_flags & OpenGLDefault)
	{
		if (!_setGLAttributes(_flags))
		{
			log = "could not set GL attributes, SDL got message: ";
			log += SDL_GetError( );
			Logger::w(log);

			return false;
		}
	}

	if (_flags & Centered)
	{
		_x = SDL_WINDOWPOS_CENTERED;
		_y = SDL_WINDOWPOS_CENTERED;
		_flags ^= Centered;
	}

	if (!_createWindow(_title, _x, _y, _width, _height, _flags))
	{
		log = "could create data SDL window, SDL got message: ";
		log += SDL_GetError( );
		Logger::w(log);
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	if (_flags & OpenGLDefault)
	{
		if (_flags & Fullscreen || _flags & FullscreenDesktop)
		{
			
		}
		if (!_createGLContext(_flags))
		{
			log = "could create data GL context, SDL got message: ";
			log += SDL_GetError( );
			Logger::w(log);
			
			return false;
		}
		if (!opengl_resources::initializeGLEW( ))
		{
			log = "could initialize GLEW";
			Logger::w(log);
			
			return false;
		}
	}

	return true;
}

void Renderer::terminate( ) noexcept
{
	if (m_window)
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
		m_glContext = nullptr;
	}
}

Renderer::Flags Renderer::windowFlags( ) const noexcept
{
	_EXPECTS(m_window, "renderer has not been initialized");

	return Flags{ SDL_GetWindowFlags(m_window) };
}

bool Renderer::setWindowFullscreen(Flags _flag) const noexcept
{
	_EXPECTS(_flag == None | _flag == Fullscreen | _flag == FullscreenDesktop,
		"flags should be None, Fullscreen or FullscreenDesktop");

	return SDL_SetWindowFullscreen(m_window, _flag) == 0;
}

void Renderer::setWindowShown( ) const noexcept
{
	_EXPECTS(m_window, "renderer has not been initialized");

	SDL_ShowWindow(m_window);
}

void Renderer::setWindowHidden( ) const noexcept
{
	_EXPECTS(m_window, "renderer has not been initialized");

	SDL_HideWindow(m_window);
}

int Renderer::windowHeight( ) const noexcept
{
	_EXPECTS(m_window, "renderer has not been initialized");
	int height;
	SDL_GetWindowSize(m_window, nullptr, &height);

	return height;
}

int Renderer::windowWidth( ) const noexcept
{
	_EXPECTS(m_window, "renderer has not been initialized");
	int width;
	SDL_GetWindowSize(m_window, &width, nullptr);

	return width;
}

const SDL_Rect Renderer::windowRect( ) const noexcept
{
	_EXPECTS(m_window, "renderer has not been initialized");
	SDL_Rect rect;
	SDL_GetWindowPosition(m_window, &rect.x, &rect.y);
	SDL_GetWindowSize(m_window, &rect.w, &rect.h);

	return rect;
}

void Renderer::draw( )
{
	glClearColor(.0f, .0f, .0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);*/
	SDL_GL_SwapWindow(m_window);
}

bool Renderer::_setGLAttributes(Flags _flags)
{
	DECL_MONOTONIC_STACK_STORGE(mem, MONOTONIC_STACK_DEF_SIZE);
	std::pmr::vector<std::pair<SDL_GLattr, int>> attrs{
		{
			std::make_pair(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE),
			std::make_pair(SDL_GL_CONTEXT_MAJOR_VERSION, 3),
			std::make_pair(SDL_GL_CONTEXT_MINOR_VERSION, 3),
			std::make_pair(SDL_GL_RED_SIZE, 8),
			std::make_pair(SDL_GL_GREEN_SIZE, 8),
			std::make_pair(SDL_GL_BLUE_SIZE, 8),
			std::make_pair(SDL_GL_ALPHA_SIZE, 8),
			std::make_pair(SDL_GL_DEPTH_SIZE, 24),
			std::make_pair(SDL_GL_DOUBLEBUFFER, 1),
			std::make_pair(SDL_GL_ACCELERATED_VISUAL, 1),
		},
		&mem
	};

	for (const auto& [attr, val] : attrs)
	{
		if (SDL_GL_SetAttribute(attr, val) < 0)
			return false;
	}

	return true;
}

bool Renderer::_createGLContext(Flags _flags)
{
	m_glContext = SDL_GL_CreateContext(m_window);

	return m_glContext;
}

bool Renderer::_createWindow(
	const std::string_view& _title,
	int _x,
	int _y,
	int _width,
	int _height,
	Flags _flags)
{
	m_window = SDL_CreateWindow(
		_title.data( ),
		_x,
		_y,
		_width,
		_height,
		_flags
	);

	return m_window;
}
