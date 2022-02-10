#include "engine_pch.h"
#include "components/rigidBody.h"
#include "components/transform.h"
#include <entt/entt.hpp>


namespace Engine
{

	void NGPhyiscs::updateTransforms()
	{
		rp3d::PhysicsWorld* world = Application::getInstance().GetWorld();		//Give application a world for this to work.
		entt::registry& reg = Application::getInstance().m_registry;
		std::vector<entt::entity>& m_entities = Application::getInstance().m_entities;
		//Log::error("BODIES - {0} ",world->getNbRigidBodies());
		 rp3d::Transform curr = world->getRigidBody(0)->getTransform();
		 Log::error("TRANSFORM - {0},{1},{2}", curr.getPosition().x, curr.getPosition().y, curr.getPosition().z);
		// world->getRigidBody(0)->setTransform(curr);
		 //auto group = reg.group<RigidBodyComponent>();
		 //for (auto entity : group)
		 //{
			// auto& transform = group.get<TransformComponent>(entity);
		 //}
		// glm::mat4 matr = curr.getOpenGLMatrix(glm::value_ptr(curr));
		 reg.replace<TransformComponent>(m_entities[1] , 
			 glm::vec3(curr.getPosition().x, curr.getPosition().y, curr.getPosition().z), glm::vec3(0), glm::vec3(1));
	}
}
