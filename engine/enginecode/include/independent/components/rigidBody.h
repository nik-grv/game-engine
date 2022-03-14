/** \file rigidBody.h */
#pragma once

#include "engine.h"	//Need to include reactphysics here i think?
#include <glm/gtc/type_ptr.hpp>
#include "systems/log.h"

namespace Engine {

	enum class RigidBodyType { Static, Kinematic, Dynamic };

	class RigidBodyComponent
	{
	public:

		//!< default constructor perhaps?
		RigidBodyComponent()

		{
			rp3d::PhysicsWorld* world = Application::getInstance().GetWorld();		//Give application a world for this to work.
			rp3d::Vector3 position(0.0, 0.0, 0.0);
			rp3d::Quaternion oritentation = rp3d::Quaternion::identity(); // Give it default orientation
			rp3d::Transform transform(position, oritentation);

			m_body = world->createRigidBody(transform);		// related to world
			m_body->setType(rp3d::BodyType::DYNAMIC);	// Set body type to be dynamic in a default constructor
			m_body->enableGravity(true);

		}


		//!< Constructor with body type
		RigidBodyComponent(RigidBodyType type)
		{
			rp3d::PhysicsWorld* world = Application::getInstance().GetWorld();		//Give application a world for this to work.
			rp3d::Vector3 position(0.0, 0.0, 0.0);
			rp3d::Quaternion oritentation = rp3d::Quaternion::identity(); // Give it default orientation
			rp3d::Transform transform(position, oritentation);

			m_body = world->createRigidBody(transform);
			switch (type)
			{
			case Engine::RigidBodyType::Static:
				m_body->setType(rp3d::BodyType::STATIC);
				break;
			case Engine::RigidBodyType::Kinematic:
				m_body->setType(rp3d::BodyType::KINEMATIC);
				break;
			case Engine::RigidBodyType::Dynamic:
				m_body->setType(rp3d::BodyType::DYNAMIC);
				break;
			default:
				m_body->setType(rp3d::BodyType::DYNAMIC);
				break;
			}
		}

		//Constructor with body type and transfortm
		RigidBodyComponent(RigidBodyType type, glm::mat4& transform, rp3d::decimal mass = 1.0, bool gravityEnabled = true)
		{
			rp3d::PhysicsWorld* world = Application::getInstance().GetWorld();		//Give application a world for this to work.
			rp3d::Transform RPtransform;
			RPtransform.setFromOpenGL(glm::value_ptr(transform));	//!< sets the transform from the passed on transform in opengl format

			auto quat = RPtransform.getOrientation();
			quat.normalize();
			RPtransform.setOrientation(quat);

			m_body = world->createRigidBody(RPtransform);
			m_body->setMass(mass);
			m_body->enableGravity(gravityEnabled);
			switch (type)
			{
			case Engine::RigidBodyType::Static:
				m_body->setType(rp3d::BodyType::STATIC);
				break;
			case Engine::RigidBodyType::Kinematic:
				m_body->setType(rp3d::BodyType::KINEMATIC);
				break;
			case Engine::RigidBodyType::Dynamic:
				m_body->setType(rp3d::BodyType::DYNAMIC);
				break;
			default:
				m_body->setType(rp3d::BodyType::DYNAMIC);
				break;
			}
		}

		RigidBodyComponent(RigidBodyType type, glm::vec3& position, glm::quat& orientation, bool gravityEnabled = true)
		{
			rp3d::PhysicsWorld* world = Application::getInstance().GetWorld();		//Give application a world for this to work.
			rp3d::Transform RPtransform;

			RPtransform.setOrientation(rp3d::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w));
			RPtransform.setPosition(rp3d::Vector3(position.x, position.y, position.z));

			m_body = world->createRigidBody(RPtransform);
			m_body->enableGravity(gravityEnabled);
			switch (type)
			{
			case Engine::RigidBodyType::Static:
				m_body->setType(rp3d::BodyType::STATIC);
				break;
			case Engine::RigidBodyType::Kinematic:
				m_body->setType(rp3d::BodyType::KINEMATIC);
				break;
			case Engine::RigidBodyType::Dynamic:
				m_body->setType(rp3d::BodyType::DYNAMIC);
				break;
			default:
				m_body->setType(rp3d::BodyType::DYNAMIC);
				break;
			}
		}

		rp3d::RigidBody* m_body;

	};

	class BoxColliderComponent {
	public:
		BoxColliderComponent(RigidBodyComponent& bodyComp, glm::vec3& halfExtents) {
			auto& physCommon = Application::getInstance().GetPhysCommon();	//needs this in application class
			auto world = Application::getInstance().GetWorld();	//needs this in application class

			shape = physCommon.createBoxShape(rp3d::Vector3(halfExtents.x, halfExtents.y, halfExtents.z));
			rp3d::Transform transform = rp3d::Transform::identity();

			collider = bodyComp.m_body->addCollider(shape, transform);
		}

		rp3d::BoxShape* shape;	//!<Box shape
		rp3d::Collider* collider = nullptr;	//!< Collider
	};

	class SphereColliderComponent {
	public:
		SphereColliderComponent(RigidBodyComponent& bodyComp, float radius)
		{
			auto& physCommon = Application::getInstance().GetPhysCommon();

			shape = physCommon.createSphereShape(radius);

			rp3d::Transform transform = rp3d::Transform::identity();

			collider = bodyComp.m_body->addCollider(shape, transform);
		}

		rp3d::SphereShape* shape;
		rp3d::Collider* collider = nullptr;
	};

	class CapsuleColliderComponent {
	public:
		CapsuleColliderComponent(RigidBodyComponent& bodyComp, float radius, float height) {
			auto& physCommon = Application::getInstance().GetPhysCommon();
			shape = physCommon.createCapsuleShape(radius, height);
			rp3d::Transform transform = rp3d::Transform::identity();
			collider = bodyComp.m_body->addCollider(shape, transform);
		}
		rp3d::CapsuleShape* shape;
		rp3d::Collider* collider = nullptr;
	};

	//Height Field Shape for terrain heightmap
	class HeightmapCollider {
	public:
		HeightmapCollider(RigidBodyComponent& bodyComp, uint32_t columCount, uint32_t rowCount, float minHeight, float maxHeight, std::vector<float> heightValues) {

			auto& physCommon = Application::getInstance().GetPhysCommon();
			shape = physCommon.createHeightFieldShape(columCount, rowCount, minHeight, maxHeight, heightValues.data(), rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);
			rp3d::Transform transform = rp3d::Transform::identity();
			collider = bodyComp.m_body->addCollider(shape, transform);
		}
		rp3d::HeightFieldShape* shape;
		rp3d::Collider* collider = nullptr;
	};


	struct DestroyOnContactComponent
	{
		bool destroyChildren = false;
	};

	class GeneralEventListener : public rp3d::EventListener
	{
	public:

		std::vector<uint32_t> toBeDestroyed;

		virtual void onContact(const CollisionCallback::CallbackData& callbackData) override
		{
			auto& registry = Application::getInstance().m_registry;
			auto& entities = Application::getInstance().m_entities;
			auto world = Application::getInstance().GetWorld();
			
			//for each contact pair entities
			for (uint32_t i = 0; i < callbackData.getNbContactPairs(); i++)
			{
				CollisionCallback::ContactPair contactPair = callbackData.getContactPair(i);

				uint32_t index1 = reinterpret_cast<uint32_t>(contactPair.getBody1()->getUserData());
				uint32_t index2 = reinterpret_cast<uint32_t>(contactPair.getBody2()->getUserData());

				entt::entity entity1 = entities[index1];
				entt::entity entity2 = entities[index2];

				bool destroy1 = registry.any_of<DestroyOnContactComponent>(entity1);
				bool destroy2 = registry.any_of<DestroyOnContactComponent>(entity2);

				if (destroy1)
					toBeDestroyed.push_back(index1);
				if (destroy2)
					toBeDestroyed.push_back(index2);

			}
		}

		void actionDestroy()
		{
			auto world = Application::getInstance().GetWorld();
			auto& registry = Application::getInstance().m_registry;
			auto& entities = Application::getInstance().m_entities;

			for (auto index : toBeDestroyed)
			{
				entt::entity entity = entities[index];
				auto& rb = registry.get<RigidBodyComponent>(entity);
				world->destroyRigidBody(rb.m_body);
				registry.destroy(entity);
			}
			toBeDestroyed.clear();
		}
	};

	namespace NGPhyiscs
	{
		void updateTransforms();
	}

}