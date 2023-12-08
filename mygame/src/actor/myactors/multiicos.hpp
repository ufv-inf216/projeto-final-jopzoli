/*
*
*/

#pragma once

#include "../actor.hpp"
#include "../component/aabbcomponent.hpp"
#include "../component/modelcomponent.hpp"

class Multiicos
	: public Actor
{
public:
	OBJIMPL( );

	Multiicos( );

	void tickActor(float) override;

private:
	object_ptr<ModelComponent> m_modelComponent;
	object_ptr<AABBComponent> m_aabbComponent;
};
OBJDECL(Multiicos, Actor);
