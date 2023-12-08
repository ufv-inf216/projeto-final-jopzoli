/*
* 
*/
#pragma once

#include "../actor.hpp"
#include "../component/aabbcomponent.hpp"
#include "../component/modelcomponent.hpp"

class Fireball
	: public Actor
{
public:
	OBJIMPL( );

	Fireball( );
    float GetLimitRange(){return mLimitRange;}
	void tickActor(float) override;

private:
	object_ptr<ModelComponent> m_modelComponent;
	object_ptr<AABBComponent> m_aabbComponent;

    float mLimitRange;
};
OBJDECL(Fireball, Actor);
