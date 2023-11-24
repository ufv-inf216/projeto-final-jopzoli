#include "scene.hpp"

Scene::Scene(Game* _game)
	: m_actors{ singleton_memory::getPool( ) }
{ }

void Scene::addActor(const not_null<Actor>& _actor)
{
	const std::type_index typeId = typeid(*_actor);
	auto it = m_actors.find(typeId);
	if (it == m_actors.end( ))
	{
		const auto& [itr, valid] = m_actors.insert(std::make_pair(
			typeId,
			std::pmr::vector<object_ptr<Actor>>{ singleton_memory::getPool( ) })
		);
		if (valid)
			it = itr;
	}

	it->second.push_back(_actor.get( ));
}

void Scene::removeActor(const not_null<Actor>& _actor)
{
	const std::type_index typeId = typeid(*_actor);
	auto it = m_actors.find(typeId);
	if (it != m_actors.end( ))
	{
		auto col = it->second;
		col.erase(std::find(col.begin( ), col.end( ), object_ptr{ _actor.get( ) }));
	}
}
