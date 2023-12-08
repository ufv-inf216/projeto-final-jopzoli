#include "arac.hpp"

ChaseState::ChaseState( )
	: FSMState{ "chase" },
	owner{ }
{ }

void ChaseState::tick(float _deltaTime)
{
	vec3 dir = owner->toMove - owner->rootComponent( )->position;
	float vel = glm::clamp(glm::length(dir), 0.f, 1.f);
	owner->getComponent<RigidBodyComponent>( )->velocity = glm::normalize(dir)
		* vec3
	{
		(vel + 1.f)* ((float) rand( ) / RAND_MAX),
		0.f,
		(vel + 1.f)* ((float) rand( ) / RAND_MAX)
	};
}

object_ptr<ModelComponent> Arac::m_modelComponent = { };

Arac::Arac( )
	: Actor{ },
	mHealth{ 100.f },
	toMove{ }
{
	if (!m_modelComponent)
	{
		m_modelComponent = object_ptr{
			new ModelComponent{ ModelLoader::load("assets/enemy.obj") }
		};
	}
	attachRoot(object_ptr{ new SceneComponent{ } });
	thisRootComponent->scale = vec3{ 1.f };

	m_aabbComponent = object_ptr{ new AABBComponent{ vec3{ }, vec3{ .4f, .4f, .4f } } };
	addComponent(m_aabbComponent);

	m_fsmComponent = object_ptr{ new FSMComponent{ } };
	addComponent(m_fsmComponent);
	object_ptr<ChaseState> chase{ new ChaseState{ } };
	chase->owner = this;
	m_fsmComponent->addState("chase", chase);
	m_fsmComponent->setCurrent("chase");

	m_modelComponent->setShader(Renderer::getCurrentShader( ));
	addComponent(m_modelComponent);

	m_rigidBodyComponent = object_ptr{ new RigidBodyComponent{ } };
	addComponent(m_rigidBodyComponent);

	activate( );
}

void Arac::tickActor(float _deltaTime)
{
	Renderer::setModelMatrix(thisRootComponent->modelMatrix( ));
	Renderer::getCurrentShader( )->setFloatVec3("objectColor", vec3{ 1.f, .6f, .7f });
}

void Arac::setHealth(float Health){
	mHealth = Health;
}
