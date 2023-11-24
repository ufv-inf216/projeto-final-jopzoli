/*
*
*/

#pragma once

#include "actor/actor.hpp"
#include "game.hpp"
#include "object/object.hpp"
#include <unordered_map>

class Scene
	: public Object
{
public:
	OBJIMPL( );

	Scene(Game* _game);

	NODISCARD bool isActive( ) const noexcept
	{
		return m_isActive;
	}

	void addActor(const not_null<Actor>& _actor);

	template <typename _IterTy>
	void addActors(_IterTy _first, _IterTy _last)
	{
		for (auto& i : std::ranges::subrange{ _first, _last })
			addActor(i);
	}

	void removeActor(const not_null<Actor>& _actor);

	template <typename _IterTy>
	void removeActors(_IterTy _first, _IterTy _last)
	{
		for (auto& i : std::ranges::subrange{ _first, _last })
			removeActor(i);
	}

	template <typename _Ty>
		requires std::derived_from<_Ty, Actor>
	void removeActors( ) noexcept
	{
		m_actors.erase(typeid(_Ty));
	}

private:
	std::pmr::map<std::type_index, std::pmr::vector<object_ptr<Actor>>> m_actors;
	bool m_isActive;
};
OBJDECL(Scene, Object);
