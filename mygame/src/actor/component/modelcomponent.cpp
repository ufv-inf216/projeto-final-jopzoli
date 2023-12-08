#include "modelcomponent.hpp"

ModelComponent::ModelComponent(meshes_type&& _meshes) noexcept
	: m_meshes{ std::move(_meshes), singleton_memory::getAssynchronousPool( ) },
	m_shader{ }
{ }

void ModelComponent::setShader(ShaderProgram* _shader) noexcept
{
	m_shader = _shader;
}

void ModelComponent::tickComponent(float _deltaTime)
{
	_EXPECTS(m_shader, "model was not configured with any shader program");
	for (auto& mesh : m_meshes)
		mesh->draw(m_shader);
}
