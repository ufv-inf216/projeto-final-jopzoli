/*
* 
*/

#pragma once

#include "math/math.hpp"
#include "component.hpp"

class RigidBodyComponent
	: public Component
{
public:
	OBJIMPL( );

	float mass;
	float friction;
	vec3 acceleration;
	vec3 velocity;

	RigidBodyComponent( );

	void applyForce(vec3 _force) noexcept;

protected:
	void tickComponent(float _deltaTime) override;
};
OBJDECL(RigidBodyComponent, Component);
