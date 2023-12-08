#include "fsmcomponent.hpp"
#include "utils/logger.hpp"

FSMState::FSMState(std::string _name)
	: Object{ },
	name{ _name }
{ }

FSMComponent::FSMComponent( )
	: Component{ },
	m_current{ "" },
	m_time{ 0.f }
{ }

void FSMComponent::addState(const std::string& _name, object_ptr<FSMState> _state) noexcept
{
	m_states[_name] = _state;
}

void FSMComponent::setCurrent(const std::string& _current)
{
	if (m_states.find(_current) != m_states.end( ))
	{
		m_time = 0.f;
		m_current = _current;
	}
}

void FSMComponent::start(const std::string& _state)
{
	m_current = _state;
}

void FSMComponent::tickComponent(float _deltaTime)
{
	m_time += _deltaTime;
	m_states[m_current]->tick(m_time);
}
