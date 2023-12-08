/*
* 
*/

#pragma once

#include "object/object.hpp"

class Component
	: public Object
{
	friend class Actor;
public:
	OBJIMPL( );

	Component( ) noexcept;

	void tick(float _deltaTime);

protected:
	virtual void tickComponent(float _deltaTime)
	{ }
};
OBJDECL(Component, Object);
