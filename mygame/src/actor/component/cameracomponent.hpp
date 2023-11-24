/*
* 
*/

#pragma once

#include "scenecomponent.hpp"

class CameraComponent
	: public SceneComponent
{
public:
	OBJIMPL( );

	// fov, aspRatio, etcs

	CameraComponent( );
};
OBJDECL(CameraComponent, SceneComponent);
