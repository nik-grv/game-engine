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
			//m_body->setMass(10000);
			//Log::error("is gravity enabled {0}", m_body->isGravityEnabled());
			//std::cout << m_body->isGravityEnabled() << std::endl;
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
		RigidBodyComponent(RigidBodyType type, glm::mat4& transform)
		{
			rp3d::PhysicsWorld* world = Application::getInstance().GetWorld();		//Give application a world for this to work.
			rp3d::Transform RPtransform;
			RPtransform.setFromOpenGL(glm::value_ptr(transform));	//!< sets the transform from the passed on transform in opengl format

			auto quat = RPtransform.getOrientation();
			quat.normalize();
			RPtransform.setOrientation(quat);

			m_body = world->createRigidBody(RPtransform);

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


	namespace NGPhyiscs
	{
		void updateTransforms();
	}

}