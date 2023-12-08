#include "component.hpp"

Component::Component( ) noexcept
	: Object{ }
{ }

void Component::tick(float _deltaTime)
{
	tickComponent(_deltaTime);
}
