/** \file physics.h */
#pragma once

#include <systems/system.h>
#include <reactphysics3d/reactphysics3d.h>

namespace Engine {
	class PhysicsSystem : public System 
	{
	public:

		rp3d::PhysicsWorld::WorldSettings worldSettings;
		//<! Starts the world "system" and creates a new world
		void start(SystemSignal init = SystemSignal::None, ...) override {
			worldSettings.gravity = rp3d::Vector3(0, -9.8, 0);
			worldSettings.worldName = "Game World";
			m_world = m_rp3dCommon.createPhysicsWorld(worldSettings);
		}

		//!< Stop world "system" and destroys the physics world
		void stop(SystemSignal close = SystemSignal::None, ...) override {
			m_rp3dCommon.destroyPhysicsWorld(m_world);
		}

		inline rp3d::PhysicsCommon& GetPhysCommon() { return m_rp3dCommon; }

		rp3d::PhysicsWorld* m_world; //!< Game world, (I think) required for all bodies and things to be in. Has gravity maybe (if not, set)

	private:
		rp3d::PhysicsCommon m_rp3dCommon;	//!< Some sort of physics common i guess. Can be accessable through application class
	};

}