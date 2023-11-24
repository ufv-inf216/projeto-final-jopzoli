#include "modelcomponent.hpp"

ModelComponent::ModelComponent(std::pmr::vector<StaticMesh>&& _meshes) noexcept
	: m_meshes{ std::move(_meshes), singleton_memory::getPool( ) },
	m_shader{ }
{ }

NODISCARD const std::pmr::vector<StaticMesh>& ModelComponent::meshes( ) const noexcept
{
	return m_meshes;
}

void ModelComponent::useShader(const not_null<ShaderProgram>& _shader) noexcept
{
	m_shader = _shader.get( );
}

void ModelComponent::tick(float _deltaTime)
{
	_draw( );
}

void ModelComponent::_draw( ) noexcept
{
	_EXPECTS(m_shader, "model was not configured with any shader program");
	for (auto& i : m_meshes)
	{
		i.draw(*m_shader);
	}
}
