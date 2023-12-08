#include "multiicos.hpp"
#include "thirdparty/renderer.hpp"

Multiicos::Multiicos( )
	: Actor{ }
{
	attachRoot(object_ptr{ new SceneComponent{ } });
	thisRootComponent->scale = vec3{ 1.f };

	m_modelComponent = object_ptr{
		new ModelComponent{ ModelLoader::load("assets/multiicos.obj") }
	};
	m_modelComponent->setShader(Renderer::getCurrentShader( ));
	addComponent(m_modelComponent);

	m_aabbComponent = object_ptr{
		new AABBComponent{ vec3{ }, vec3{ 0.4f } }
	};
	addComponent(m_aabbComponent);

	activate( );
}

void Multiicos::tickActor(float)
{
	Renderer::setModelMatrix(thisRootComponent->modelMatrix( ));
	Renderer::getCurrentShader( )->setFloatVec3("objectColor", vec3{ 0.8f, 0.6f, 0.9f });
}
