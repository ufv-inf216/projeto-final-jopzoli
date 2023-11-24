#include "actor.hpp"
#include "utils/logger.hpp"

Actor::Actor( ) noexcept
	: Object( ),
	thisComponents{ singleton_memory::getPool( ) },
	thisRootComponent{ },
	thisStatus{ Paused }
{ }

Actor::~Actor( ) noexcept
{ }

void Actor::attachRoot(const not_null<SceneComponent>& _component) noexcept
{
	thisRootComponent = _component.get( );
}

void Actor::detachRoot( ) noexcept
{
	thisRootComponent = nullptr;
}

void Actor::addComponent(const not_null<Component>& _component)
{
	const std::type_index typeId = typeid(*_component);
	_WARNING(!thisComponents.contains(typeId), "overriding existing component on actor");
	thisComponents[typeId] = _component.get( );
	_component->setOwner(this);
}

void Actor::removeComponent(const not_null<Component>& _component) noexcept
{
	thisComponents.erase(typeid(*_component));
	_component->setOwner(nullptr);
}

void Actor::setOwner(Object* _owner) noexcept
{
	thisOwner = _owner;
}

void Actor::setState(State _state) noexcept
{
	thisStatus = _state;
}

void Actor::tick(float _deltaTime)
{
	tickComponents(_deltaTime);
	tickActor(_deltaTime);
}

void Actor::tickActor(float _deltaTime)
{ }

void Actor::tickComponents(float _deltaTime)
{
	for (auto& [_, component] : thisComponents)
	{
		if (component->m_active)
			component->tick(_deltaTime);
	}
}
