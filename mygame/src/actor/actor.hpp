/*
*
*/

#pragma once

#include "component/component.hpp"
#include "component/scenecomponent.hpp"
#include "object/object.hpp"
#include <map>

class Scene;

class Actor
	: public Object
{
public:
	OBJIMPL( );

	enum State
	{
		Active,
		Paused,
		Dead
	};

	Actor( ) noexcept;

	virtual ~Actor( ) noexcept;

	NODISCARD const std::pmr::map<std::type_index, object_ptr<Component>>& components( ) const noexcept
	{
		return thisComponents;
	}

	NODISCARD SceneComponent* rootComponent( ) const noexcept
	{
		return thisRootComponent.get( );
	}

	NODISCARD State state( ) const noexcept
	{
		return thisStatus;
	}

	template <typename _Ty>
	NODISCARD _Ty* getComponent( ) const
	{
		static_assert(std::is_base_of_v<Component, _Ty>, "given type is not data Component");
		auto it = components.find(typeid(_Ty));
		if (it != components.end( ))
			return rtti_dynamic_cast<_Ty*>(it->second.get( ));

		_WARNING(it != components.end( ), "trying to get unexistent component");

		return nullptr;
	}

	void attachRoot(const not_null<SceneComponent>& _component) noexcept;

	void detachRoot( ) noexcept;

	void addComponent(const not_null<Component>& _component);

	void removeComponent(const not_null<Component>& _component) noexcept;

	template <typename _Ty>
	void removeComponent( ) noexcept
	{
		static_assert(std::is_base_of_v<Component, _Ty>, "given type is not data Component");

		auto it = components.find(typeid(_Ty));
		if (it != components.end( ))
		{
			it->second->setOwner(nullptr);
			components.erase(it);
		}
	}

	void setOwner(Object* _owner) noexcept;

	void setState(State _state) noexcept;

	void tick(float _deltaTime);

protected:
	std::pmr::map<std::type_index, object_ptr<Component>> thisComponents;
	object_ptr<SceneComponent> thisRootComponent;
	State thisStatus;

	virtual void tickActor(float _deltaTime);

	void tickComponents(float _deltaTime);
};
OBJDECL(Actor, Object);
