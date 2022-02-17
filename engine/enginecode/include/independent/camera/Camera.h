/** \file Camera.h*/
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
	\class Camera
	* \brief class to make the camera move on user input
	*/
	class Camera
	{
	public:
		Camera(); //!< constructor
		void setCameraPos(glm::vec3 pos);
		inline glm::vec3 getCameraPos() { return m_cameraPos; }
		void mouseMovement(double xpos, double ypos); //!< move camera from mouse movement
		void mouseScroll(double yOffset); //!< zoom camera on mouse scroll
		void update(float deltaTime); //!< update time
		void smoothZMovement(float deltaTime); //!< function to interpolate smooth Z movement 
		void smoothXMovement(float deltaTime); //!< function to interpolate smooth X movement 
		glm::mat4 circleCamera();				//!< auto camera circular movement
		inline glm::mat4 getCameraViewMatrix() const {
			return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
		} //!< get the view matrix
		inline float getFOV() const { return m_FOV; } //!< get new FOV 
	private:
		glm::vec3 m_cameraPos; //!< start camera position
		glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //!< camera front vector
		glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //!< camera up vector
		float m_FOV = 45.0f; //!< default FOV
		float m_SpeedMultiplierZ = 2.5f; //!< camera speed movement m_multiplier in Z axis
		float pressedMultiplierZ = 0.0f; //!< key press time for smooth interpolation when stopping in Z axis
		float startPressMultiplierZ = 0.0f;//!< key press time for smooth interpolation at start in Z axis
		
		float m_SpeedMultiplierX = 2.5f; //!< camera speed movement m_multiplier in X axis
		float pressedMultiplierX = 0.0f; //!< key press time for smooth interpolation when stopping in X axis
		float startPressMultiplierX = 0.0f;//!< key press time for smooth interpolation at start in X axis
		
		const float m_CameraSpeedIncreaseMultiplier = 0.1f; //!< increase normal camera speed per frame by this value when key held down
		const float m_keyPressThreshold = 0.001f;//!< max lowest value when stopping smooth movement
		const float m_maxKeyPress = 0.2f; //!< max key press value achievable 
		const float m_smoothFactor = 3.0f; //!< smooothness factor ( more smoothness = more distance covered after stop
		const float m_maxStartMultiplier= 0.2f; //!< max smooothness for start
		const float m_StartSmoothFactor = 0.1f; //!< smooothness factor ( less smoothness = more distance covered when starting to move)

		bool W_keyPressed = false; //!< is W key pressed
		bool S_keyPressed = false;//!< is S key pressed
		bool A_keyPressed = false;//!< is A key pressed
		bool D_keyPressed = false;//!< is D key pressed
		
		bool m_firstMouse = true; //!< mose movement check
		float m_yaw = -90.0f;	//!< initialized m_yaw as -90.0 to rotate to the left.
		float m_pitch = 0.0f; //!< m_pitch for mouse movement
		float m_lastX = static_cast<unsigned int>(1024) / 2.0f; //!< x of mouse
		float m_lastY = static_cast<unsigned int>(800) / 2.0f; //!< y of mouse
	};

}