#include "scenecomponent.hpp"

SceneComponent::SceneComponent( ) noexcept
	: Component( ),
	position{ 0.f },
	rotation{ vec3{ 0.f } },
	scale{ 1.f }
{ }

NODISCARD mat4 SceneComponent::modelMatrix(mat4 _mat) const noexcept
{
	_mat = glm::translate(_mat, position);
	_mat = glm::scale(_mat, scale);
	_mat *= glm::toMat4(rotation);

	return _mat;
}
