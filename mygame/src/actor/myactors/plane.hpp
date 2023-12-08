/*
* 
*/

#pragma once

#include "../component/modelcomponent.hpp"
#include "../actor.hpp"

class Plane
	: public Actor
{
public:
	OBJIMPL( );

	vec3 offset;

	Plane( );

protected:
	void tickActor(float _deltaTime) override;

private:
	object_ptr<ModelComponent> m_modelComponent;
};
OBJDECL(Plane, Actor);
