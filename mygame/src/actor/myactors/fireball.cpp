#include "fireball.hpp"
#include "thirdparty/renderer.hpp"

Fireball::Fireball( )
	: Actor{ },
    mLimitRange{20.f}
{
	attachRoot(object_ptr{ new SceneComponent{ } });
	thisRootComponent->scale = vec3{ 1.f };

	m_modelComponent = object_ptr{
		new ModelComponent{ ModelLoader::load("assets/icos.obj") }
	};
	m_modelComponent->setShader(Renderer::getCurrentShader( ));
	addComponent(m_modelComponent);

	m_aabbComponent = object_ptr{
		new AABBComponent{ vec3{ }, vec3{ 0.15f } }
	};
	addComponent(m_aabbComponent);

	activate( );
}

void Fireball::tickActor(float)
{
	Renderer::setModelMatrix(thisRootComponent->modelMatrix( ));
	Renderer::getCurrentShader( )->setFloatVec3("objectColor", vec3{ 0.4f, 0.3f, 0.5f });
}
