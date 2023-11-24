#include "game.hpp"
#include "thirdparty/input.hpp"
#include "thirdparty/shaderprogram.hpp"
#include "utils/logger.hpp"
#include "actor/myactors/test.hpp"
#include <chrono>

using namespace std::chrono_literals;

Game::Game( )
	: m_eventFilter{ },
	m_renderer{ },
	status{ Halted },
	camera{ (float) m_renderer.windowWidth( ) / (float) m_renderer.windowHeight( ),
	45.f,
	1.f,
	100.f,
	vec3{ 0.f, 0.f, 0.f },
	vec3{ 0.f, 1.f, 0.f } }
{
	m_renderer.initialize("my game | v0.1", 0, 0, 1024, 568,
		Renderer::Centered /*| Renderer::FullscreenDesktop*/ | Renderer::OpenGLDefault);
	_registerEventHandlers( );
	camera.getCameraComponent( )->position = vec3{ 1.f, 1.f, 1.f };
}

Game::~Game( )
{
	m_renderer.terminate( );
	SDL_SAFE_TERMINATE( );
}

NODISCARD Game::RuntimeStatus Game::state( ) noexcept
{
	return status;
}

NODISCARD Renderer& Game::renderer( ) noexcept
{
	return m_renderer;
}

NODISCARD EventFilter& Game::eventFilter( ) noexcept
{
	return m_eventFilter;
}

namespace
{
void GLAPIENTRY glerro(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	Logger::l("gl got error");
}
}

void Game::run( )
{
	ShaderProgram shader1 = ShaderProgram::create(basic_vert, basic_frag);
	ShaderProgram shader2 = ShaderProgram::create(notex_vert, notex_frag);
	/*ShaderProgram basicShader{ ShaderProgram::create(basic_vert, basic_frag) };
	auto mesh = ModelLoader::load("C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/src/assets/backpack.obj");*/

	/*TestActor test1{ &shader };
	test1.rootComponent( )->scale = vec3{ .2f };
	test1.rootComponent( )->position = vec3{ 0.f, 0.f, 0.f };
	TestActor test2{ &shader };
	test2.rootComponent( )->scale = vec3{ .2f };
	test2.rootComponent( )->position = vec3{ 0.f, 0.05f, 0.f };*/
	//TestActor actor;
	auto a = ModelLoader::load(
		"C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/src/assets/teapot.obj");
	auto b = ModelLoader::load(
		"C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/src/assets/teapot.obj");
	auto c = ModelLoader::load(
		"C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/src/assets/teapot.obj");
	auto e = ModelLoader::load(
		"C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/src/assets/teapot.obj");
	auto d = ModelLoader::load(
		"C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/src/assets/backpack.obj");

	glEnable(GL_DEPTH_TEST);

	// render loop
	// -----------
	float time = 0.f;
	while (status != Quit)
	{
		m_eventFilter.sendEvents( );
		if (status == Paused)
		{
			std::this_thread::sleep_for(1us);
			continue;
		}
		// input
		// -----

		// render
		// ------
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader2.use( );
		shader2.setFloatMat4("projection", camera.getProjectionMatrix( ));
		shader2.setFloatMat4("view", camera.getViewMatrix( ));
		auto model = glm::translate(mat4{ 1.f }, vec3{ 0.f });
		model = glm::scale(model, vec3{ 4.f });
		shader2.setFloatMat4("model", model);
		for (auto& i : a)
		{
			i.draw(shader1);
		}
		shader2.use( );
		shader2.setFloatMat4("projection", camera.getProjectionMatrix( ));
		shader2.setFloatMat4("view", camera.getViewMatrix( ));
		model = glm::translate(mat4{ 1.f }, vec3{ -0.1f, 0.f, 0.1f });
		model = glm::scale(model, vec3{ 4.f });
		shader2.setFloatMat4("model", model);
		for (auto& i : b)
		{
			i.draw(shader2);
		}
		shader2.use( );
		shader2.setFloatMat4("projection", camera.getProjectionMatrix( ));
		shader2.setFloatMat4("view", camera.getViewMatrix( ));
		model = glm::translate(mat4{ 1.f }, vec3{ 0.1f, 0.f, 0.1f });
		model = glm::scale(model, vec3{ 4.f });
		shader2.setFloatMat4("model", model);
		for (auto& i : c)
		{
			i.draw(shader2);
		}
		shader1.use( );
		shader1.setFloatMat4("projection", camera.getProjectionMatrix( ));
		shader1.setFloatMat4("view", camera.getViewMatrix( ));
		model = glm::translate(mat4{ 1.f }, vec3{ -0.1f, 0.f, -0.1f });
		model = glm::scale(model, vec3{ .05f });
		shader1.setFloatMat4("model", model);
		for (auto& i : d)
		{
			i.draw(shader2);
		}
		shader2.use( );
		shader2.setFloatMat4("projection", camera.getProjectionMatrix( ));
		shader2.setFloatMat4("view", camera.getViewMatrix( ));
		model = glm::translate(mat4{ 1.f }, vec3{ 0.2f, 0.f, -0.4f });
		model = glm::scale(model, vec3{ 4.f });
		shader2.setFloatMat4("model", model);
		for (auto& i : e)
		{
			i.draw(shader2);
		}
		/*test1.setProjectionMatrix(camera.getProjectionMatrix( ));
		test1.setViewMatrix(camera.getViewMatrix( ));
		test1.tick(time);
		test2.setProjectionMatrix(camera.getProjectionMatrix( ));
		test2.setViewMatrix(camera.getViewMatrix( ));
		test2.tick(-time);*/
		//time += 0.01f;
		// draw our first triangle

		//actor.tick(time);

		SDL_GL_SwapWindow(m_renderer.window( ));
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
		Logger::i(log);
		break;
	}
	}
}

void Game::_onKeyboardEvent(SDL_KeyboardEvent& _e)
{
	float prec = 0.005;
	keyboard_event_trigger::whenKeyDown(_e, SDLK_F11).then([&] {
		if (m_renderer.windowFlags( ) & Renderer::Fullscreen)
			m_renderer.setWindowFullscreen( );
		else
			m_renderer.setWindowFullscreen(Renderer::FullscreenDesktop);
		});
	keyboard_event_trigger::whenKeyDown(_e, SDLK_a, 0, true).then([&] {
		camera.forward.x -= prec;
		camera.forward.z += prec;
		camera.getCameraComponent( )->position.x -= prec;
		camera.getCameraComponent( )->position.z += prec;
		//camera.getCameraComponent( )->position.z -= 0.05;
		});
	keyboard_event_trigger::whenKeyDown(_e, SDLK_d, 0, true).then([&] {
		camera.forward.x += prec;
		camera.forward.z -= prec;
		camera.getCameraComponent( )->position.x += prec;
		camera.getCameraComponent( )->position.z -= prec;
		});
}

void Game::_onMouseButtonEvent(SDL_MouseButtonEvent& _e)
{
	std::string log;
	switch (_e.type)
	{
		/*case SDL_MOUSEBUTTONDOWN :
		{
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			break;
		}*/
	default:
	{
		log += "Got unhandled SDL mouse button event of type: ";
		log += std::to_string(_e.type);
		Logger::i(log);
		break;
	}
	}
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
	Logger::i("Got SDL quit event with passed ticks: "
		+ std::to_string(_e.timestamp));
	status = Quit;
}

void Game::_onSysWMEvent(SDL_SysWMEvent& _e)
{
	/*std::string log;
	log += "Got unhandled SDL syswm event";
	Logger::i(log);*/
}

void Game::_onWindowEvent(SDL_WindowEvent& _e)
{
	window_event_trigger::when(_e, SDL_WINDOWEVENT_HIDDEN).then([&] {
		status = Paused;
		Logger::d("Window hidden, game paused");
		});
	window_event_trigger::when(_e, SDL_WINDOWEVENT_MINIMIZED).then([&] {
		status = Paused;
		Logger::d("Window minimized, game paused");
		});
	window_event_trigger::when(_e, SDL_WINDOWEVENT_EXPOSED).then([&] {
		status = Running;
		Logger::d("Window exposed, game running");
		});
	/*window_event_trigger::when(_e, SDL_WINDOWEVENT_RESTORED).then([&] {
		m_state = Running;
		Logger::d("Window retored, game running");
		});*/
	window_event_trigger::when(_e, SDL_WINDOWEVENT_MOVED).then([&] {
		status = Paused;
		Logger::d("Window moved, game paused");
		});
	window_event_trigger::when(_e, SDL_WINDOWEVENT_RESIZED).then([&] {
		status = Paused;
		Logger::d("Window resized, game paused");
		});
}
