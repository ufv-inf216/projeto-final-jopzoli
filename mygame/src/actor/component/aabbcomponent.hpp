/*
* 
*/

#pragma once

#include "component.hpp"
#include "math/math.hpp"

class AABBComponent
	: public Component
{
public:
	OBJIMPL( );

	struct Overlap
	{
		vec3 dist;
		AABBComponent* target;
	};

	AABBComponent(vec3 _offset, vec3 _box);

	vec3 getMin( ) const noexcept;

	vec3 getMax( ) const noexcept;

	vec3 getCenter( ) const noexcept;

	bool intersect(const AABBComponent& _b) const noexcept;

private:
	vec3 m_offset;
	vec3 m_box;
};
OBJDECL(AABBComponent, Component);
