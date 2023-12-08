/*
* 
*/

#pragma once

#include "../actor.hpp"
#include "../component/aabbcomponent.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "thirdparty/shaderprogram.hpp"

class Mage
	: public Actor
{
public:
	OBJIMPL( );

	quat relativeRot;
	object_ptr<ShaderProgram> shader;

	Mage( );

	void tickActor(float _deltaTime) override;
	float GetHealth(){return mHealth;}
	void setHealth(float Health);

private:
	object_ptr<ModelComponent> m_modelComponent;
	object_ptr<CameraComponent> m_cameraComponent;
	object_ptr<AABBComponent> m_aabbComponent;
	object_ptr<RigidBodyComponent> m_rigidBodyComponent;
	float m_lastTime;
	vec3 m_tilt;
	float mHealth;
};
OBJDECL(Mage, Actor);
