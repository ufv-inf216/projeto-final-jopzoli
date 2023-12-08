/*
* 
*/

#pragma once

#include "component.hpp"
#include <map>

class FSMState
	: public Object
{
public:
	OBJIMPL( );

	std::string name;

	FSMState(std::string _name);

	virtual void tick(float _deltaTime)
	{ }
};
OBJDECL(FSMState, Object);

class FSMComponent
	: public Component
{
public:
	OBJIMPL( );

	FSMComponent( );

	void addState(const std::string& _name, object_ptr<FSMState> _state) noexcept;

	void setCurrent(const std::string& _current);

	void start(const std::string& _state);

protected:
	void tickComponent(float _deltaTime) override;

private:
	std::string m_current;
	std::map<std::string, object_ptr<FSMState>> m_states;
	float m_time;
};
OBJDECL(FSMComponent, Component);
