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

	ModelComponent(std::pmr::vector<StaticMesh>&& _meshes) noexcept;

	NODISCARD const std::pmr::vector<StaticMesh>& meshes( ) const noexcept;

	void useShader(const not_null<ShaderProgram>& _shader) noexcept;

	void tick(float _deltaTime) override;

private:
	std::pmr::vector<StaticMesh> m_meshes;
	ShaderProgram* m_shader;

	void _draw( ) noexcept;
};
OBJDECL(ModelComponent, Component);
