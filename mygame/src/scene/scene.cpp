#include "scene.hpp"

Scene::Scene(Game* _game)
	: Object{ },
	actors{ singleton_memory::getAssynchronousPool( ) },
	game{ _game },
	m_status{ Inactive }
{ }

void Scene::play( ) noexcept
{
	m_status = Active;
	onPlay( );
}

void Scene::pause( ) noexcept
{
	m_status = Inactive;
	onPause( );
}

void Scene::tick(float _deltaTime)
{
	if (m_status == Active)
	{
		tickScene(_deltaTime);
		_tickActors(_deltaTime);
	}
}

void Scene::_tickActors(float _deltaTime)
{
	for (auto& [name, actor] : actors)
		actor->tick(_deltaTime);
}
