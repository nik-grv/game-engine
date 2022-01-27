/** \file Camera.cpp

*\brief The Camera Class file
*
* Sets up a camera's movement by defnining keyboard and mosue controls
*/

#include <engine_pch.h>
#include <camera/Camera.h>
namespace Engine
{
	//! Empty Camera Constructor
	/*!
	*/
	Camera::Camera()
	{
	}

	//! Function to set the camera position at the start of the scene
	/*!
	\param position glm::vec3 - vec3 positon
	*/
	void Camera::setCameraPos(glm::vec3 pos)
	{
		m_cameraPos = pos;
	}


	//! Function to set the mouse position
	/*!
	\param position double - position in x axis
	\param position double - position in y axis
	*/
	void Camera::mouseMovement(double xpos, double ypos)
	{
		if (m_firstMouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_firstMouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos;
		m_lastX = xpos;
		m_lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_cameraFront = glm::normalize(front);

	}


	//! Function to set the mouse scroll 
	/*!
	\param offset double - scroll offset
	*/
	void Camera::mouseScroll(double yOffset)
	{
		m_FOV += (float)yOffset;
		Log::error(m_FOV);
		if (m_FOV < 1.0f)
			m_FOV = 1.0f;
		if (m_FOV > 90.0f)
			m_FOV = 90.0f;
	}


	//! Function to update the camera position each frame
	/*!
	\param deltaTime float - time between each frame
	*/
	void Camera::update(float deltaTime)
	{
		float m_deltaTime = deltaTime;

		if (InputPoller::isKeyPressed(NG_KEY_W))
		{
			W_keyPressed = true;
			S_keyPressed = false;

			smoothZMovement(m_deltaTime);
		}
		if (InputPoller::isKeyPressed(NG_KEY_S))
		{
			W_keyPressed = false;
			S_keyPressed = true;

			smoothZMovement(m_deltaTime);
		}
		
		if (InputPoller::isKeyPressed(NG_KEY_A))
		{
			D_keyPressed = false;
			A_keyPressed = true;

			smoothXMovement(m_deltaTime);
		}
		if (InputPoller::isKeyPressed(NG_KEY_D))
		{
			D_keyPressed = true;
			A_keyPressed = false;

			smoothXMovement(m_deltaTime);
		}

		//key release functions....
		if (InputPoller::isKeyReleased(NG_KEY_W) && InputPoller::isKeyReleased(NG_KEY_S))
		{
			m_SpeedMultiplierZ = 2.5f;

			if (W_keyPressed || S_keyPressed)
			{
				if (pressedMultiplierZ > m_keyPressThreshold)
				{
					pressedMultiplierZ -= m_deltaTime * pressedMultiplierZ * m_smoothFactor;
					if(W_keyPressed)
					m_cameraPos += pressedMultiplierZ * m_cameraFront;
					else
						m_cameraPos -= pressedMultiplierZ * m_cameraFront;
				}
				else
				{
					W_keyPressed = false;
					S_keyPressed = false;
					startPressMultiplierZ = 0.0f;
					pressedMultiplierZ = 0.0f;
				}
			}
		}
		
		if (InputPoller::isKeyReleased(NG_KEY_A) && InputPoller::isKeyReleased(NG_KEY_D))
		{
			m_SpeedMultiplierX = 2.5f;

			if (A_keyPressed || D_keyPressed)
			{
				if (pressedMultiplierX > m_keyPressThreshold)
				{
					glm::vec3 left = glm::cross(m_cameraFront, m_cameraUp);
					pressedMultiplierX -= m_deltaTime * pressedMultiplierX * m_smoothFactor;
					if(A_keyPressed)
					m_cameraPos -= left * pressedMultiplierX;
					else
						m_cameraPos += left * pressedMultiplierX;
				}
				else
				{
					A_keyPressed = false;
					D_keyPressed = false;
					pressedMultiplierX = 0.0f;
					startPressMultiplierX = 0.0f;
				}
			}
		}
	}


	//! Function to set smooth movement in Z axis
	/*!
	\param deltaTime float - time between each frame
	*/
	void Camera::smoothZMovement(float deltaTime)
	{
		float cameraSpeed = m_SpeedMultiplierZ * deltaTime;

		if(m_SpeedMultiplierZ < 20.0f)
			m_SpeedMultiplierZ += 0.1f;
		
		if (pressedMultiplierZ <= m_maxKeyPress)
			pressedMultiplierZ += 0.001f;

		if (startPressMultiplierZ < cameraSpeed && startPressMultiplierZ < m_maxStartMultiplier)
		{
			startPressMultiplierZ += (deltaTime + startPressMultiplierZ) * m_StartSmoothFactor;
			if (W_keyPressed)
				m_cameraPos += startPressMultiplierZ * m_cameraFront;
			else
				m_cameraPos -= startPressMultiplierZ * m_cameraFront;
		}
		else
		{
			if (W_keyPressed)
				m_cameraPos += cameraSpeed * m_cameraFront;
			else
				m_cameraPos -= cameraSpeed * m_cameraFront;

		}
	}


	//! Function to set smooth movement in X axis
	/*!
	\param deltaTime float - time between each frame
	*/
	void Camera::smoothXMovement(float deltaTime)
	{
		float cameraSpeed = m_SpeedMultiplierX * deltaTime;

		if (m_SpeedMultiplierX < 20.0f)
			m_SpeedMultiplierX += 0.1f;

		if (pressedMultiplierX <= m_maxKeyPress)
			pressedMultiplierX += 0.001f;

		glm::vec3 left = glm::cross(m_cameraFront, m_cameraUp);

		if (startPressMultiplierX < cameraSpeed && startPressMultiplierX < m_maxStartMultiplier)
		{
			startPressMultiplierX += (deltaTime + startPressMultiplierX) * m_StartSmoothFactor;
			
			if(D_keyPressed)
				m_cameraPos += left * startPressMultiplierX;
			else
				m_cameraPos -= left * startPressMultiplierX;
		}
		else
		{
			if (D_keyPressed)
				m_cameraPos += left * cameraSpeed;
			else
				m_cameraPos -= left * cameraSpeed;
		}
	}



	//! Function to set continuous circulae camera movement
	/*!
	*/
	glm::mat4 Camera::circleCamera()
	{
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ - 3.0f), glm::vec3(0.0, 0.0, -6.0), glm::vec3(0.0, 1.0, 0.0));
		return view;
	}
}