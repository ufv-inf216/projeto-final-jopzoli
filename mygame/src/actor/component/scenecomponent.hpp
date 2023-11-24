/*
* 
*/

#pragma once

#include "component.hpp"
#include "math/math.hpp"
#include <glm/gtx/quaternion.hpp>

class SceneComponent
	: public Component
{
public:
	OBJIMPL( );

	vec3 position;
	quat rotation;
	vec3 scale;

	SceneComponent( ) noexcept { }
};
OBJDECL(SceneComponent, Component);
