/*
* 
*/

#pragma once

#include "../actor.hpp"
#include "../component/modelcomponent.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestActor
	: public Actor
{
public:
	ShaderProgram* shader{ };

	TestActor(ShaderProgram* _shader)
		: shader{ _shader }
	{
		auto model = new ModelComponent(ModelLoader::load(
			"C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/src/assets/backpack.obj"));
		model->useShader(shader);
		addComponent(model);
		thisRootComponent = new SceneComponent( );
		thisRootComponent->position = vec3{ 0.f };
		thisRootComponent->scale = vec3{ 1.f };
		thisRootComponent->rotation = quat{ vec3{ 0.f } };
	}

	void setProjectionMatrix(const mat4 _projection) const noexcept
	{
		shader->setFloatMat4("projection", _projection);
	}

	void setViewMatrix(const mat4 _view) const noexcept
	{
		shader->setFloatMat4("view", _view);
	}

	void setModelMatrix(mat4 _model = mat4(1.f)) const noexcept
	{
		_model = glm::scale(_model, thisRootComponent->scale);
		_model = glm::translate(_model, thisRootComponent->position);
		_model *= glm::toMat4(thisRootComponent->rotation);
		shader->setFloatMat4("model", _model);
	}

protected:
	void tickActor(float _deltaTime) override
	{
		thisRootComponent->rotation = glm::rotate(thisRootComponent->rotation, glm::radians(_deltaTime), vec3{ 0.f, 1.f, 0.f });
		setModelMatrix( );
	}
};
