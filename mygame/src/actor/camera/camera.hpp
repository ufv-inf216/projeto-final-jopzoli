/*
* 
*/

#pragma once

#include "../actor.hpp"
#include "../component/cameracomponent.hpp"

class Camera
	: public Actor
{
public:
	OBJIMPL( );

	float aspectRatio;
	float fov;
	float near;
	float far;
	vec3 forward;
	vec3 up;

	Camera(float _aspectRatio, float _fov, float _near, float _far, vec3 _forward, vec3 _up) noexcept
		: aspectRatio{ _aspectRatio },
		fov{ _fov },
		near{ _near },
		far{ _far },
		forward{ _forward },
		up{ _up }
	{
		thisRootComponent = new CameraComponent( );
	}

	NODISCARD CameraComponent* getCameraComponent( ) noexcept
	{
		return thisRootComponent.get( );
	}

	NODISCARD mat4 getProjectionMatrix( ) const noexcept
	{
		return glm::perspective(glm::radians(fov), aspectRatio, near, far);
	}

	NODISCARD mat4 getViewMatrix( ) const noexcept
	{
		_EXPECTS(thisRootComponent, "camera does not have a valid root component");
		//auto pos = glm::rotate(thisRootComponent->rotation, thisRootComponent->position);

		return glm::lookAt(thisRootComponent->position, forward, up);
	}

private:
	object_ptr<CameraComponent> thisRootComponent;
};
OBJDECL(Camera, Actor);
