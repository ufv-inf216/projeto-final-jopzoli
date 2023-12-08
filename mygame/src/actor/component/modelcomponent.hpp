/*
* 
*/

#pragma once

#include "component.hpp"
#include "thirdparty/modelloader.hpp"

class ModelComponent
	: public Component
{
public:
	OBJIMPL( );

	using meshes_type = std::pmr::vector<object_ptr<StaticMesh>>;

	ModelComponent(meshes_type&& _meshes) noexcept;

	NODISCARD const std::pmr::vector<object_ptr<StaticMesh>>& meshes( ) const noexcept
	{
		return m_meshes;
	}

	NODISCARD ShaderProgram* shader( ) const noexcept
	{
		return m_shader;
	}

	void setShader(ShaderProgram* _shader) noexcept;

protected:
	void tickComponent(float _deltaTime) override;

private:
	meshes_type m_meshes;
	ShaderProgram* m_shader;
};
OBJDECL(ModelComponent, Component);
