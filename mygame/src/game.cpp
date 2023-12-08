#include "actor/actor.hpp"
#include "game.hpp"
#include "actor/component/cameracomponent.hpp"
#include "scene/myscenes/mainscene.hpp"
#include "thirdparty/input.hpp"
#include "thirdparty/modelloader.hpp"
#include "thirdparty/shaderprogram.hpp"
#include "utils/logger.hpp"
#include <chrono>

using namespace std::chrono_literals;

Game::Game( )
	: m_eventFilter{ },
	m_renderer{ },
	m_ticksPassed{ },
	m_status{ Halted }
{
	m_renderer.initialize("Survive", 0, 0, 1024, 568,
		Renderer::Centered /*| Renderer::FullscreenDesktop*/ | Renderer::OpenGLDefault);
	m_renderer.shader = object_ptr{ ShaderProgram::create(notex_vert, notex_frag) };
	Renderer::current = &m_renderer;
	_registerEventHandlers( );
	m_currentScene = object_ptr{ new MainScene{ this } };
	m_currentScene->play( );
}

Game::~Game( )
{
	m_renderer.terminate( );
	SDL_SAFE_TERMINATE( );
}

NODISCARD Game::RuntimeStatus Game::state( ) noexcept
{
	return m_status;
}

NODISCARD Renderer& Game::renderer( ) noexcept
{
	return m_renderer;
}

NODISCARD EventFilter& Game::eventFilter( ) noexcept
{
	return m_eventFilter;
}

void Game::quit( ) noexcept
{
	m_status = Quit;
}

void Game::run( )
{
	glEnable(GL_DEPTH_TEST);
	float time = 0.f;
	auto last = std::chrono::milliseconds(0);
	m_ticksPassed = SDL_GetTicks( );
	while (m_status != Quit)
	{
		m_eventFilter.sendEvents( );
		if (m_status == Inactive)
		{
			std::this_thread::sleep_for(1us);
			continue;
		}
		
		while (!SDL_TICKS_PASSED(SDL_GetTicks( ), m_ticksPassed + 16));

		float deltaTime = (SDL_GetTicks( ) - m_ticksPassed) / 1000.0f;
		if (deltaTime > 0.05f)
			deltaTime = 0.05f;

		m_renderer.beginDraw( );
		m_currentScene->tick(deltaTime);
		m_renderer.endDraw( );

		m_ticksPassed = SDL_GetTicks( );
	}
}

void Game::_registerEventHandlers( )
{
	auto onAudioDeviceEvent = [&](SDL_Event& _e) { _onAudioDeviceEvent(_e.adevice); };
	m_eventFilter.registerHandler(EventFilter::AudioDeviceAdded, onAudioDeviceEvent);
	m_eventFilter.registerHandler(EventFilter::AudioDeviceRemoved, onAudioDeviceEvent);

	auto onKeyboardEvent = [&](SDL_Event& _e) { _onKeyboardEvent(_e.key); };
	m_eventFilter.registerHandler(EventFilter::KeyDown, onKeyboardEvent);
	m_eventFilter.registerHandler(EventFilter::KeyUp, onKeyboardEvent);

	auto onMouseButtonEvent = [&](SDL_Event& _e) { _onMouseButtonEvent(_e.button); };
	m_eventFilter.registerHandler(EventFilter::MouseButtonDown, onMouseButtonEvent);
	m_eventFilter.registerHandler(EventFilter::MouseButtonUp, onMouseButtonEvent);

	auto onMouseMotionEvent = [&](SDL_Event& _e) { _onMouseMotionEvent(_e.motion); };
	m_eventFilter.registerHandler(EventFilter::MouseMotion, onMouseMotionEvent);

	auto onMouseWheelEvent = [&](SDL_Event& _e) { _onMouseWheelEvent(_e.wheel); };
	m_eventFilter.registerHandler(EventFilter::MouseWheel, onMouseWheelEvent);

	auto onQuitEvent = [&](SDL_Event& _e) { _onQuitEvent(_e.quit); };
	m_eventFilter.registerHandler(EventFilter::Quit, onQuitEvent);

	auto onSysWMEvent = [&](SDL_Event& _e) { _onSysWMEvent(_e.syswm); };
	m_eventFilter.registerHandler(EventFilter::SysWMEvent, onSysWMEvent);

	auto onWindowEvent = [&](SDL_Event& _e) { _onWindowEvent(_e.window); };
	m_eventFilter.registerHandler(EventFilter::WindowEvent, onWindowEvent);
}

void Game::_onAudioDeviceEvent(SDL_AudioDeviceEvent& _e)
{
	std::string log;
	switch (_e.type)
	{
		/*case SDL_AUDIODEVICEADDED:
		{
			break;
		}
		case SDL_AUDIODEVICEREMOVED:
		{
			break;
		}*/
	default:
	{
		log += "Got unhandled SDL audio device event of type: ";
		log += std::to_string(_e.type);
		Logger::info(log);
		break;
	}
	}
}

void Game::_onKeyboardEvent(SDL_KeyboardEvent& _e)
{
	m_currentScene->onKeyboardEvent(_e);
}

void Game::_onMouseButtonEvent(SDL_MouseButtonEvent& _e)
{
	m_currentScene->onMouseButtonEvent(_e);
}

void Game::_onMouseMotionEvent(SDL_MouseMotionEvent& _e)
{
	/*std::string log;
	log += "Got unhandled SDL mouse motion event";
	Logger::i(log);*/
}

void Game::_onMouseWheelEvent(SDL_MouseWheelEvent& _e)
{
	/*std::string log;
	log += "Got unhandled SDL mouse wheel event";
	Logger::i(log);*/
}

void Game::_onQuitEvent(SDL_QuitEvent& _e)
{
	Logger::info("Got SDL quit event with passed ticks: "
		+ std::to_string(_e.timestamp));
	m_status = Quit;
}

void Game::_onSysWMEvent(SDL_SysWMEvent& _e)
{
	/*std::string log;
	log += "Got unhandled SDL syswm event";
	Logger::i(log);*/
}

void Game::_onWindowEvent(SDL_WindowEvent& _e)
{
	m_currentScene->onWindowEvent(_e);
}
