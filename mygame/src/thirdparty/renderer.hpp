/*
* 
*/

#pragma once

#include "thirdparty.hpp"
#include "math/math.hpp"
#include "shaderprogram.hpp"
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

	struct display_info
	{
		int index;
		SDL_Rect resolution;
		uint32_t frameRate;
	};

	static Renderer* current;

	static Renderer* getCurrent( ) noexcept
	{
		_EXPECTS(current, "no current renderer was set");
		return current;
	}

	static ShaderProgram* getCurrentShader( ) noexcept
	{
		_EXPECTS(getCurrent( )->shader, "no current shader was set");
		return getCurrent()->shader.get( );
	}

	static void setPerspectiveMatrix(mat4 _matrix) noexcept;

	static void setViewMatrix(mat4 _matrix) noexcept;

	static void setModelMatrix(mat4 _matrix) noexcept;

	object_ptr<ShaderProgram> shader;

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

	int windowHeight( ) const noexcept;

	int windowWidth( ) const noexcept;

	const SDL_Rect windowRect( ) const noexcept;

	const display_info displayInfo( ) const noexcept;

	bool setWindowFullscreen(Flags _flag = None) const noexcept;

	void setWindowShown( ) const noexcept;

	void setWindowHidden( ) const noexcept;

	void beginDraw( );

	void endDraw( );

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
