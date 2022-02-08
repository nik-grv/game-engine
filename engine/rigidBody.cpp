#include "engine_pch.h"
#include "components/rigidBody.h"


namespace Engine
{

	void NGPhyiscs::updateTransforms()
	{
		rp3d::PhysicsWorld* world = Application::getInstance().GetWorld();		//Give application a world for this to work.

		Log::error("BODIES - {0} ",world->getNbRigidBodies());
		 rp3d::Transform curr = world->getRigidBody(0)->getTransform();
		 Log::error("TRANSFORM - {0},{1},{2}", curr.getPosition().x, curr.getPosition().y, curr.getPosition().z);
		 world->getRigidBody(0)->setTransform(curr);
	}
}
