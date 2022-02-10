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
		auto group = reg.view<TransformComponent,RigidBodyComponent>();

		for (auto entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			rp3d::Transform currTransform = group.get<RigidBodyComponent>(entity).m_body->getTransform();
			transform.translation = glm::vec3(currTransform.getPosition().x, currTransform.getPosition().y, currTransform.getPosition().z);

			//getting the rotation with a quaternion and setting it to a quat
			transform.rotation = glm::quat(currTransform.getOrientation().w, currTransform.getOrientation().x, currTransform.getOrientation().y, currTransform.getOrientation().z);
			transform.UpdateTransform();
		}
	}
}
