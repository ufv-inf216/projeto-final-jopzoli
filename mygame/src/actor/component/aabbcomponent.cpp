#include "aabbcomponent.hpp"
#include "actor/actor.hpp"

AABBComponent::AABBComponent(vec3 _offset, vec3 _box)
	: Component{ },
	m_offset{ _offset },
	m_box{ _box }
{ }

vec3 AABBComponent::getMin( ) const noexcept
{
	return rtti_dynamic_cast<Actor*>(thisOwner)->rootComponent( )->position - m_box / 2.f + m_offset;
}

vec3 AABBComponent::getMax( ) const noexcept
{
	return getMin( ) + m_box;
}

vec3 AABBComponent::getCenter( ) const noexcept
{
	return getMin( ) + m_box / 2.f;
}

bool AABBComponent::intersect(const AABBComponent& _b) const noexcept
{
	vec3 aMin = getMin( ), aMax = getMax( ),
		bMin = _b.getMin( ), bMax = _b.getMax( );
	return aMin.x <= bMax.x &&
		aMax.x >= bMin.x &&
		aMin.y <= bMax.y &&
		aMax.y >= bMin.y &&
		aMin.z <= bMax.z &&
		aMax.z >= bMin.z;
}
