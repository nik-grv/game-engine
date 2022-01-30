/** \file physics.h */
#pragma once

#include <systems/system.h>
#include <reactphysics3d/reactphysics3d.h>

namespace Engine {
	class PhysicsSystem : public System {
	public:
		//<! Starts the world "system" and creates a new world
		void start(SystemSignal init = SystemSignal::None, ...) override {
			m_world = m_rp3dCommon.createPhysicsWorld();
		}

		//!< Stop world "system" and destroys the physics world
		void stop(SystemSignal close = SystemSignal::None, ...) override {
			m_rp3dCommon.destroyPhysicsWorld(m_world);
		}

		rp3d::PhysicsWorld* m_world; //!< Game world, (I think) required for all bodies and things to be in. Has gravity maybe (if not, set)
	private:
		rp3d::PhysicsCommon m_rp3dCommon;	//!< Some sort of physics common i guess. Can be accessable through application class
	};
}