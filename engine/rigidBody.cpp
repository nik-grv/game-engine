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

		auto group = reg.view<TransformComponent,RigidBodyComponent>();
		int i = world->getNbRigidBodies() - 1;
		for (auto entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			rp3d::Transform currTransform = world->getRigidBody(i)->getTransform();
			reg.replace<TransformComponent>(entity,
				glm::vec3(currTransform.getPosition().x, currTransform.getPosition().y, currTransform.getPosition().z)
				, glm::vec3(transform.rotation.x, transform.rotation.y, transform.rotation.z), glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
			i--;
		}
	}
}
