#include "testactor.hpp"

TestActor::TestActor( )
	: Actor{ }
{
	shader = object_ptr<ShaderProgram>{
		ShaderProgram::create(notex_vert, notex_frag)
	};
	thisRootComponent = object_ptr{ new SceneComponent{ } };
	thisRootComponent->scale = vec3{ 1.f };

	m_modelComponent = object_ptr{
		new ModelComponent{ ModelLoader::load(
			"C:/Users/secjo/OneDrive/Documentos/Projects/inf216-tpfinal/mygame/"
			"src/assets/mage.obj") }
	};
	m_modelComponent->setShader(shader);
	addComponent(m_modelComponent);

	m_cameraComponent = object_ptr{
		new CameraComponent{ CameraComponent::Perspective }
	};
	m_cameraComponent->position = vec3{ 2.f };
	activate( );
}

void TestActor::tickActor(float _deltaTime)
{
	/*m_cameraComponent->position.x = thisRootComponent->position.x;
	m_cameraComponent->position.z = thisRootComponent->position.z;*/
	shader->use( );
	shader->setFloatMat4("projection", m_cameraComponent->projectionMatrix( ));
	shader->setFloatMat4("view", m_cameraComponent->viewMatrix( ));
	shader->setFloatMat4("model", thisRootComponent->modelMatrix( ));
}
