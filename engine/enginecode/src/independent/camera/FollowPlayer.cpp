/** \file FollowPlayer.cpp

*\brief The FollowPlayer class for a camera
*
* Sets up a follow camera around a cube
*/

#include <engine_pch.h>
#include "camera/FollowPlayer.h"


namespace Engine
{

	//! FollowPlayer Constructor
	//! Set the players' model matrix to be followed
	/*!
	\param modelMatrix glm::mat4 - the model matrix of the player object
	*/
	FollowPlayer::FollowPlayer(glm::mat4& playerModel) : m_playerModel(playerModel)
	{
		m_offset = glm::vec3(0.0f, 2.0f, 5.0f);
	}

	//! Separate function to set the players' model matrix to be followed
	/*!
	\param modelMatrix glm::mat4 - the model matrix of the player object
	*/
	void FollowPlayer::setPlayer(glm::mat4& playerModel)
	{
		m_playerModel = playerModel;
	}

	//! Update the camera position
	/*!
	\param deltaTime float - time between each frame
	*/
	void FollowPlayer::onUpdate(float deltaTime)
	{
		m_deltaTime = deltaTime;
		
		glm::vec3 camRight(m_playerModel[0][0], m_playerModel[0][1], m_playerModel[0][2]);
		glm::vec3 camUp(m_playerModel[1][0], m_playerModel[1][1], m_playerModel[1][2]);
		glm::vec3 camForward(m_playerModel[2][0], m_playerModel[2][1], m_playerModel[2][2]);
		glm::vec3 playerPos = glm::vec3(m_playerModel[3][0], m_playerModel[3][1], m_playerModel[3][2]);

		glm::vec3 deltaOffset(0.0f);
		deltaOffset += camRight * getOffset().x;
		deltaOffset += camUp * getOffset().y;
		deltaOffset += camForward * getOffset().z;

		glm::vec3 camPos = playerPos + deltaOffset;

		glm::vec3 deltaPos = playerPos - camPos;
		glm::vec3 upVector = glm::cross(camRight, deltaPos);

		m_viewMatrix = glm::lookAt(camPos, playerPos,upVector);
	}

	//! Set offset between camera and the player model
	/*!
	\param distance glm::vec3 - distance between camera and player
	*/
	void FollowPlayer::setOffset(glm::vec3 offset)
	{
		m_offset = offset;
	}
}