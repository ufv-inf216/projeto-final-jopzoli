/*
* 
*/

#pragma once

#include "scenecomponent.hpp"

constexpr float R1_1 = 1.f;
constexpr float R3_2 = 1.5f;
constexpr float R4_3 = 1.33333333f;
constexpr float R5_4 = 1.25f;
constexpr float R15_9 = 1.66666666f;
constexpr float R16_9 = 1.77777777f;
constexpr float R16_10 = 1.6f;

class CameraComponent
	: public SceneComponent
{
public:
	OBJIMPL( );

	static constexpr float Min_fov = 0.26179938f;
	static constexpr float Max_fov = 2.35619449f;
	static constexpr float Min_near = .00001f;
	static constexpr float Inf_far = std::numeric_limits<float>::infinity( );

	enum ProjectionMode
	{
		Perspective,
	};

	vec3 target{ 0.f };

	CameraComponent(
		ProjectionMode _projectionMode,
		float _fieldOfView = PiO2,
		float _aspectRatio = R16_9,
		float _near = .1f,
		float _far = 100.f) noexcept;

	NODISCARD ProjectionMode projectionMode( ) const noexcept
	{
		return m_projectionMode;
	}

	NODISCARD float fieldOfView( ) const noexcept
	{
		m_fieldOfView;
	}

	NODISCARD float aspectRatio( ) const noexcept
	{
		return m_aspectRatio;
	}

	NODISCARD float near( ) const noexcept
	{
		return m_near;
	}

	NODISCARD float far( ) const noexcept
	{
		return m_far;
	}

	NODISCARD mat4 projectionMatrix( ) const noexcept;

	NODISCARD mat4 viewMatrix( ) const noexcept;

	void setFieldOfView(float _fov) noexcept;

	void setAspectRatio(float _ratio) noexcept;

	void setNear(float _near) noexcept;

	void setFar(float _far) noexcept;

	void setTarget(const vec3 _target) noexcept;

protected:

private:
	ProjectionMode m_projectionMode;
	float m_fieldOfView;
	float m_aspectRatio;
	float m_near;
	float m_far;
};
OBJDECL(CameraComponent, SceneComponent);
