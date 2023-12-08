/*
* 
*/

#pragma once

#include "../actor.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "thirdparty/shaderprogram.hpp"

class TestActor
	: public Actor
{
public:
	OBJIMPL( );

	object_ptr<ShaderProgram> shader;

	TestActor( );

	void tickActor(float _deltaTime) override;

private:
	object_ptr<ModelComponent> m_modelComponent;
	object_ptr<CameraComponent> m_cameraComponent;
};
OBJDECL(TestActor, Actor);