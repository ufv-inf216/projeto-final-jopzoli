#include "rigidbodycomponent.hpp"
#include "actor/actor.hpp"

RigidBodyComponent::RigidBodyComponent( )
	: mass{ 1.f },
	friction{ 0.09f },
	velocity{ },
	acceleration{ }
{ }

void RigidBodyComponent::applyForce(vec3 _force) noexcept
{
	acceleration += _force * (1.f / mass);
}

void RigidBodyComponent::tickComponent(float _deltaTime)
{
	vec3& pos = rtti_dynamic_cast<Actor*>(thisOwner)->rootComponent( )->position;

	velocity += acceleration * _deltaTime;

	auto newPos = glm::toMat4(rtti_dynamic_cast<Actor*>(thisOwner)->rootComponent( )->rotation)
		* vec4 { velocity, 0.f } * _deltaTime;
	pos.x += newPos.x;
	pos.y += newPos.y;
	pos.z += newPos.z;
}
