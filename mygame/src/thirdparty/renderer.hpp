/*
* 
*/

#pragma once

#include "thirdparty.hpp"
#include <string_view>
#include <vector>

#define RENDERER_DEFAULT_WIDTH 1024
#define RENDERER_DEFAULT_HEIGHT 576

class Renderer
{
public:
	enum Flags
		: Uint32
	{
		None = SDL_FALSE,
		Fullscreen = SDL_WINDOW_FULLSCREEN,
		FullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
		OpenGLDefault = SDL_WINDOW_OPENGL,
		Shown = SDL_WINDOW_SHOWN,
		Hidden = SDL_WINDOW_HIDDEN,
		Borderless = SDL_WINDOW_BORDERLESS,
		Resizable = SDL_WINDOW_RESIZABLE,
		Minimized = SDL_WINDOW_MINIMIZED,
		Maximized = SDL_WINDOW_MAXIMIZED,
		GrabbedInput = SDL_WINDOW_INPUT_GRABBED,
		FocusInput = SDL_WINDOW_INPUT_FOCUS,
		FocusMouse = SDL_WINDOW_MOUSE_FOCUS,
		Centered = 0x40000000,
	};

	Renderer( ) noexcept;

	~Renderer( );

	NODISCARD bool isInitialized( ) const noexcept
	{
		return m_window;
	}

	bool initialize(
		const std::string_view& _title,
		int x,
		int y,
		int width,
		int height,
		Flags _flags);

	void terminate( ) noexcept;

	NODISCARD SDL_GLContext glContext( ) const noexcept
	{
		return m_glContext;
	}

	NODISCARD SDL_Window* window( ) const noexcept
	{
		return m_window;
	}

	Flags windowFlags( ) const noexcept;

	bool setWindowFullscreen(Flags _flag = None) const noexcept;

	void setWindowShown( ) const noexcept;

	void setWindowHidden( ) const noexcept;

	int windowHeight( ) const noexcept;

	int windowWidth( ) const noexcept;

	const SDL_Rect windowRect( ) const noexcept;

	void draw( );

private:
	SDL_GLContext m_glContext;
	SDL_Window* m_window;

	bool _setGLAttributes(Flags _flags);
	bool _createGLContext(Flags _flags);
	bool _createWindow(
		const std::string_view& _title,
		int _x,
		int _y,
		int _width,
		int _height,
		Flags _flags);
};
OP_BITWISE_COMPAT_(Renderer::Flags);
