#include "mage.hpp"
#include "thirdparty/renderer.hpp"

Mage::Mage( )
	: Actor{ },
	relativeRot{ vec3{ 0.f } },
	m_lastTime{ 0.f },
	mHealth{100.f},
	m_tilt{ }
{
	attachRoot(object_ptr{ new SceneComponent{ } });
	thisRootComponent->scale = vec3{ 1.f };

	m_modelComponent = object_ptr{
		new ModelComponent{ ModelLoader::load("assets/mage.obj") }
	};
	m_modelComponent->setShader(Renderer::getCurrentShader( ));
	addComponent(m_modelComponent);

	m_cameraComponent = object_ptr{
		new CameraComponent{ CameraComponent::Perspective, glm::radians(45.f), R16_9, 0.1f, 40.f }
	};
	m_cameraComponent->position = vec3{ -1.f, 4.f, 0.5f };
	addComponent(m_cameraComponent);

	m_aabbComponent = object_ptr{
		new AABBComponent{ vec3{ }, vec3{ 0.4f, 0.6f, 0.4f } }
	};
	addComponent(m_aabbComponent);

	m_rigidBodyComponent = object_ptr{
		new RigidBodyComponent{ }
	};
	addComponent(m_rigidBodyComponent);

	activate( );
}

void Mage::tickActor(float _deltaTime)
{
	m_lastTime += _deltaTime;
	m_cameraComponent->position.x = thisRootComponent->position.x - 1.f;
	m_cameraComponent->position.z = thisRootComponent->position.z + 0.5f;
	m_cameraComponent->target.x = thisRootComponent->position.x;
	m_cameraComponent->target.z = thisRootComponent->position.z;
	Renderer::setPerspectiveMatrix(m_cameraComponent->projectionMatrix( ));
	Renderer::setViewMatrix(m_cameraComponent->viewMatrix( ));
	Renderer::setModelMatrix(thisRootComponent->modelMatrix( ));
	Renderer::getCurrentShader( )->setFloatVec3("viewPos", m_cameraComponent->position);
	Renderer::getCurrentShader( )->setFloatVec3("lightPos", m_cameraComponent->position);
	Renderer::getCurrentShader( )->setFloatVec3("objectColor", vec3{ 1.f, 1.f, 1.f });
	thisRootComponent->position.y = (1.f + glm::sin(m_lastTime * 3.f)) / 4.f;

	
	float vel = glm::length(m_rigidBodyComponent->velocity);
	float angle = vel / 1.5f * glm::radians(25.f);

	m_rigidBodyComponent->velocity = glm::clamp(
		m_rigidBodyComponent->velocity,
		vec3{ },
		vec3{ 1.5f });

	thisRootComponent->rotation = relativeRot * glm::rotate(quat{ vec3{ } }, angle, vec3{ 1.f, 0.f, 0.f });
}

void Mage::setHealth(float Health){
	mHealth = Health;
}