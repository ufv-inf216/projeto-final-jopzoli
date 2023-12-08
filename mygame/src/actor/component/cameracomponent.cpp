#include "cameracomponent.hpp"

CameraComponent::CameraComponent(
	ProjectionMode _projectionMode,
	float _aspectRatio,
	float _fieldOfView,
	float _near,
	float _far) noexcept
	: SceneComponent{ },
	m_projectionMode{ _projectionMode }
{
	setFieldOfView(_fieldOfView);
	setAspectRatio(_aspectRatio);
	setNear(_near);
	setFar(_far);
}

mat4 CameraComponent::projectionMatrix( ) const noexcept
{
	mat4 mat;
	switch (m_projectionMode)
	{
	case Perspective:
	{
		mat = m_far != Inf_far
			? glm::perspective(
				m_fieldOfView,
				m_aspectRatio,
				m_near,
				m_far)
			: glm::infinitePerspective(
				m_fieldOfView,
				m_aspectRatio,
				m_near);
		break;
	}
	default:
	{
		mat = mat4{ 1.f };
		break;
	}
	}

	return mat;
}

mat4 CameraComponent::viewMatrix( ) const noexcept
{
	return glm::lookAt(
		position,
		target,
		glm::rotate(rotation, vec3{ 0.f, 1.f, 0.f }));
}

void CameraComponent::setFieldOfView(float _fov) noexcept
{
	m_fieldOfView = glm::clamp(_fov, Min_fov, Max_fov);
}

void CameraComponent::setAspectRatio(float _ratio) noexcept
{
	_WARNING(_ratio != 0.f, "setting camera aspect ratio to zero");
	m_aspectRatio = _ratio;
}

void CameraComponent::setNear(float _near) noexcept
{
	m_near = glm::max(_near, Min_near);
}

void CameraComponent::setFar(float _far) noexcept
{
	m_far = _far;
}

void CameraComponent::setTarget(const vec3 _target) noexcept
{
	target = _target;
}
