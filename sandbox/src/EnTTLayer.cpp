#include "EnTTLayer.h"

namespace Engine {


	EnTTLayer::EnTTLayer(const char* name) : Layer(name), m_registry(Application::getInstance().m_registry), m_entities(Application::getInstance().m_entities)
	{
		m_entities.resize(4);
		m_entities[0] = m_registry.create();
		m_entities[1] = m_registry.create();
		m_entities[2] = m_registry.create();
		m_entities[3] = m_registry.create();

		//needs entities
		//add to registry
		//add render component
		//add transform component
	}

	//for loop to render all in render function
}
