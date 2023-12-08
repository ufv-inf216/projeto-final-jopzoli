/*
* 
*/

#pragma once

#include "../actor.hpp"
#include "../component/aabbcomponent.hpp"
#include "../component/modelcomponent.hpp"

class Icosaedro
	: public Actor
{
public:
	OBJIMPL( );

	Icosaedro( );

	void tickActor(float) override;

private:
	object_ptr<ModelComponent> m_modelComponent;
	object_ptr<AABBComponent> m_aabbComponent;
};

OBJDECL(Icosaedro, Actor);
