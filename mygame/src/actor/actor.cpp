#include "actor.hpp"
#include "utils/logger.hpp"

Actor::Actor( ) noexcept
	: Object{ },
	thisRootComponent{ },
	m_components{ singleton_memory::getAssynchronousPool( ) },
	m_status{ Inactive }
{ }

void Actor::addComponent(const object_ptr<Component>& _component)
{
	const std::type_index typeId = typeid(*_component);
	_WARNING(!m_components.contains(typeId),
		"overriding existing component on actor");
	m_components[typeId] = _component;
	_component->thisOwner = this;
}

void Actor::removeComponent(const not_null<Component>& _component) noexcept
{
	m_components.erase(typeid(*_component));
	_component->thisOwner = nullptr;
}

void Actor::tick(float _deltaTime)
{
	if (m_status == Active)
	{
		tickActor(_deltaTime);
		_tickRootComponent(_deltaTime);
		_tickComponents(_deltaTime);
	}
}

void Actor::_tickRootComponent(float _deltaTime)
{
	if (thisRootComponent)
		thisRootComponent->tick(_deltaTime);
}

void Actor::_tickComponents(float _deltaTime)
{
	for (auto& [tid, component] : m_components)
		component->tick(_deltaTime);
}
