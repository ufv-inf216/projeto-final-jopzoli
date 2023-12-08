#include "plane.hpp"
#include "thirdparty/renderer.hpp"

Plane::Plane( )
	: Actor{ }
{
	attachRoot(object_ptr{ new SceneComponent{ } });

	/*m_modelComponent = object_ptr{
		new ModelComponent{ ModelLoader::load("assets/plane.obj") }
	};
	addComponent(m_modelComponent);*/

	activate();
}

void Plane::tickActor(float _deltaTime)
{
}
