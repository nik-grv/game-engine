/** \file FollowPlayer.h*/
#pragma once

#include "systems/log.h"
#include "core/timer.h"

#include "systems/log.h"
#include "events/events.h"
#include "core/inputPoller.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace Engine {

	/**
	\class FollowPlayer
	* \brief class to make the camera follow the player (cube)
	*/
	class FollowPlayer
	{
	public:
		FollowPlayer(glm::mat4& playerModel); //!< constructor
		void setPlayer(glm::mat4& playerModel); //!<set the player positon
		void onUpdate(float deltaTime); //!< move camera on update
		void setOffset(glm::vec3 offset); //!< set offset
		inline glm::mat4 getViewMatrix() const { return m_viewMatrix; } //!< get the view matrix
		inline glm::vec3 getOffset() const { return m_offset; } //!< get the offset
	private:
		float m_deltaTime; //!< time elapsed 
		glm::mat4& m_playerModel; //!< my player model ref
		glm::vec3 m_offset; //!< offset betwen the player and camera
		glm::mat4 m_viewMatrix; //!< camera view matrrix
	};
}