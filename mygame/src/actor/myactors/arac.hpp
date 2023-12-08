/*
*
*/

#pragma once

#include "../actor.hpp"
#include "../component/aabbcomponent.hpp"
#include "../component/fsmcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "thirdparty/renderer.hpp"

class ChaseState
	: public FSMState
{
public:
	OBJIMPL( );

	class Arac* owner;

	ChaseState( );

	void tick(float _deltaTime) override;
};
OBJDECL(ChaseState, FSMState);

class Arac
	: public Actor
{
public:
	OBJIMPL( );

	vec3 toMove;

	Arac( );
	float GetHealth(){return mHealth;}
	void setHealth(float Health);
	void tickActor(float _deltaTime) override;

private:
	static object_ptr<ModelComponent> m_modelComponent;
	object_ptr<AABBComponent> m_aabbComponent;
	object_ptr<FSMComponent> m_fsmComponent;
	object_ptr<RigidBodyComponent> m_rigidBodyComponent;
	float mHealth;
};
OBJDECL(Arac, Actor);
