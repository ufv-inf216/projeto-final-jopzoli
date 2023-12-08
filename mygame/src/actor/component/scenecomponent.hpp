/*
* 
*/

#pragma once

#include "component.hpp"
#include "math/math.hpp"

class SceneComponent
	: public Component
{
public:
	OBJIMPL( );

	vec3 position;
	quat rotation;
	vec3 scale;

	SceneComponent( ) noexcept;

	NODISCARD mat4 modelMatrix(mat4 _mat = mat4{ 1.f }) const noexcept;
};
OBJDECL(SceneComponent, Component);
