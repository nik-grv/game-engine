/** \file application.cpp
*\brief The Application Class file
* Contains all the code for creating the window, rendering object and camera movements
*/
#include "engine_pch.h"
#include <glad/glad.h>
#include <cassert>
#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWSystem.h"
#endif

#include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>


#include "rendering/subTexture.h"

#include "rendering/IndexBuffer.h"
#include "rendering/VertexBuffer.h"
#include "rendering/VertexArray.h"
#include "rendering/ShaderRend.h"
#include "rendering/UniformBuffer.h"

#include "rendering/Renderer3D.h"
#include "rendering/Renderer2D.h"

namespace Engine {
	// Set static vars
	Application* Application::s_instance = nullptr;

	//! Constructor to setup the window and the keyboard events 
	/*!
	*/
	Application::Application()
	{
		for (auto ent : m_entities)
			ent = m_registry.create();
		
		if (s_instance == nullptr)
		{
			s_instance = this;
		}
		//Start system and logger
		m_loggerSystem.reset(new Log); 
		m_loggerSystem->start();

		//start windows system
#ifdef NG_PLATFORM_WINDOWS
		m_windowSystem.reset(new GLFWSystem);
#endif
		m_windowSystem->start();

		m_physics.reset(new PhysicsSystem);
		m_physics->start(); // reset first? we need?
		//m_physics->m_world->setGravity(reactphysics3d::Vector3(0.f, -10.f, 0.f));
		//m_physics->m_world->setIsGravityEnabled(true);

		//reset timer
		m_timer.reset(new ChronoTimer);
		m_timer->start();


#pragma region [FEATURE] Set Fullsceen
		/*while (!m_fullscreenSet)
		{
			std::cout << "\nStart with Fullscreen (Y/N) ?\n";
			std::cin >> m_setFullScreen;
			
			while (tolower(m_setFullScreen) != 'y' && tolower(m_setFullScreen) != 'n')
			{
				std::cout << "\nPlease enter Y or N as ur choice : ";
				std::cin >> m_setFullScreen;
			}

			if (tolower(m_setFullScreen) == 'y')
			{
				m_isFullscreen = true;
				m_fullscreenSet = true;
				std::cout << "\nLaunching in Fullscreen Mode!\n";
				std::cin.ignore();
			}
			else
			{
				m_isFullscreen = false;
				m_fullscreenSet = true;
				std::cout << "\nLaunching in Minimised Mode!\n";
				std::cin.ignore();
			}
		
		}
		*/
#pragma endregion

		WindowProperties props("My Game Engine",RendererShared::SCR_WIDTH, RendererShared::SCR_HEIGHT,m_isFullscreen);
		m_window.reset(Window::create(props));

		//window callbacks
		m_window->getEventHandler().setOnCloseCallback(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		m_window->getEventHandler().setOnResizeCallback(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMovedCallback(std::bind(&Application::onWindowMoved, this, std::placeholders::_1));
		m_window->getEventHandler().setOnLostFocusCallback(std::bind(&Application::onWindowLostFocus, this, std::placeholders::_1));
		m_window->getEventHandler().setOnFocusCallback(std::bind(&Application::onWindowFocus, this, std::placeholders::_1));
		//keyboard callbacks
		m_window->getEventHandler().setOnKeyPressedEvent(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnKeyReleasedEvent(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));
		
		//mouse callbacks
		m_window->getEventHandler().setOnMouseMovedEvent(std::bind(&Application::onMouseMoved, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseBtnPressedEvent(std::bind(&Application::onMouseBtnPressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseBtnReleasedEvent(std::bind(&Application::onMouseBtnReleased, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseScrolledEvent(std::bind(&Application::onMouseScrolled, this, std::placeholders::_1));

		InputPoller::setNativeWindow(m_window->getNativewindow());
		m_timer->reset();

		Renderer2D::init();
		//Renderer3D::init();

	}

#pragma region AppEvents
	/*!
	\param closeEvent WindowCloseEvent  - window close event
	*/
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		e.handle(true);
		m_running = false;
		return e.isHandled();
	}

	/*!
	\param resizeEvent WindowResizeEvent  - window resize event
	*/
	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		e.handle(true);
		auto& size = e.getSize();
	//	Log::trace("Window Resize : {0} / {1} ",size.x,size.y);
		return e.isHandled();
	}


	/*!
	\param movedEvent WindowMovedEvent  - window moved event
	*/
	bool Application::onWindowMoved(WindowMovedEvent& e)
	{
		e.handle(true);
		auto& size = e.getPos();
		Log::trace("Window Moved : {0} / {1}", size.x, size.y);
		return e.isHandled();
	}


	/*!
	\param focusEvent WindowLostFocusEvent  - window lost focus event
	*/
	bool Application::onWindowLostFocus(WindowLostFocusEvent& e)
	{
		e.handle(true);
		m_layerStack.SetAllActive(false);
		//Log::trace("Lost Focus");
		return e.isHandled();
	}

	/*!
	\param focusEvent WindowFocusEvent  - window gained focus event
	*/
	bool Application::onWindowFocus(WindowFocusEvent& e)
	{
		e.handle(true);
		m_layerStack.SetAllActive(true);
		//Log::trace("Has Focus");
		return e.isHandled();
	}

	/*!
	\param keyEvent KeyPressedEvent  - keyboard key press event
	*/
	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		e.handle(true);
		Log::trace("{0} Key Pressed ", e.getKeyCode());
		if (e.getKeyCode() == NG_KEY_ESCAPE)
		{
			glfwSetInputMode(reinterpret_cast<GLFWwindow*>(m_window->getNativewindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (e.getKeyCode() == NG_KEY_SPACE)
		{
			//switch to player cam....
			isPlayerCam = !isPlayerCam;
		}

		m_layerStack.onKeyPressed(e);
		return e.isHandled();
	}

	/*!
	\param keyEvent KeyReleasedEvent  - keyboard key released event
	*/
	bool Application::onKeyReleased(KeyReleasedEvent& e)
	{
		e.handle(true);
		//reset speed multiplier for both cameras on key release
		if (e.getKeyCode() == NG_KEY_W || e.getKeyCode() == NG_KEY_A || e.getKeyCode() == NG_KEY_S || e.getKeyCode() == NG_KEY_D
			|| e.getKeyCode() == NG_KEY_Q || e.getKeyCode() == NG_KEY_E || e.getKeyCode() == NG_KEY_Z || e.getKeyCode() == NG_KEY_X)
			m_SpeedMultiplier = 2.5f;

		m_layerStack.onKeyReleased(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseMovedEvent  - mouse moved event
	*/
	bool Application::onMouseMoved(MouseMovedEvent& e)
	{
		e.handle(true);
		auto& size = e.getMousePos();
		//m_camera.mouseMovement(size.x, size.y);
		//Log::trace("Mouse Moved : {0} / {1}", size.x, size.y);
		m_layerStack.onMouseMoved(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseButtonPressedEvent  - mouse button press event
	*/
	bool Application::onMouseBtnPressed(MouseButtonPressedEvent& e)
	{
		e.handle(true);
		if (e.getButton() == 0)
		{
			glfwSetInputMode(reinterpret_cast<GLFWwindow*>(m_window->getNativewindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		m_layerStack.onMouseBtnPressed(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseButtonReleasedEvent  - mouse button release event
	*/
	bool Application::onMouseBtnReleased(MouseButtonReleasedEvent& e)
	{
		e.handle(true);
		//Log::trace("Mouse Button Released : {0} ", e.getButton());
		m_layerStack.onMouseBtnReleased(e);

		return e.isHandled();
	}

	/*!
	\param mouseEvent MouseScrollEvent  - mouse scroll event
	*/
	bool Application::onMouseScrolled(MouseScrollEvent& e)
	{
		e.handle(true);
		m_camera.mouseScroll(e.getYScroll());
		projection =
			glm::perspective(glm::radians(m_camera.getFOV()), (float)RendererShared::SCR_WIDTH / (float)RendererShared::SCR_HEIGHT, 0.1f, 100.f);

		m_layerStack.onMouseScrolled(e);

		return e.isHandled();
	}

#pragma endregion

	Application::~Application()
	{
		//delete world
		m_physics->stop();
		//stop the systems and logger
		m_loggerSystem->stop();
		//stop windows system
		m_windowSystem->stop();
		
		for (auto ent : m_entities)
			m_registry.destroy(ent);
	}


	//! Defines the main loop functions of the application. 
	//! Code for rendering, camera movement, etc run in the function
	/*!
	*/
	void Application::run()
	{

#pragma region [History] - TEXTURES
/*
		std::shared_ptr<TextureRend> letterTexture;
		letterTexture.reset(TextureRend::create("assets/textures/letterCube.png"));
		std::shared_ptr<TextureRend> numberTexture;
		numberTexture.reset(TextureRend::create("assets/textures/numberCube.png"));
		std::shared_ptr<TextureRend> letterAndNumTexture;
		letterAndNumTexture.reset(TextureRend::create("assets/textures/letterAndNumber.png"));

		SubTexture letterSubTex(letterAndNumTexture, { 0.0f,0.0f }, { 1.0f,0.5f });
		SubTexture A_Letter(letterAndNumTexture, { 0.0f,0.0f }, { 0.33f,0.25f });
		SubTexture numberSubTex(letterAndNumTexture, { 0.f,0.5f }, { 1.0f,1.0f });

		std::shared_ptr<TextureRend> plainWhiteTex;
		unsigned char whitePixel[4] = { 255,255,255,255 };
		plainWhiteTex.reset(TextureRend::create(1, 1, 4, whitePixel));
		*/
		

#pragma endregion

		//Renderer3D::init();

#pragma region [History] - RAW_DATA

		////vertices data when using texture atlast
//float cubeVertices[8 * 24] = {
//	//	 <------ Pos ------>  <--- normal --->  <-- UV -->
//		 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  letterSubTex.transformU(0.f),  letterSubTex.transformV(0.f),
//		 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  letterSubTex.transformU(0.f),  letterSubTex.transformV(0.5f),
//		-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  letterSubTex.transformU(0.33f),letterSubTex.transformV(0.5f),
//		-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  letterSubTex.transformU(0.33f),letterSubTex.transformV(0.f),

//		-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  letterSubTex.transformU(0.33f),letterSubTex.transformV(0.5f),
//		 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  letterSubTex.transformU(0.66f),letterSubTex.transformV(0.5f),
//		 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  letterSubTex.transformU(0.66f),letterSubTex.transformV(0.f),
//		-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  letterSubTex.transformU(0.33), letterSubTex.transformV(0.f),

//		-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  letterSubTex.transformU(1.f),  letterSubTex.transformV(0.f),
//		 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  letterSubTex.transformU(0.66f),letterSubTex.transformV(0.f),
//		 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  letterSubTex.transformU(0.66f),letterSubTex.transformV(0.5f),
//		-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  letterSubTex.transformU(1.0f), letterSubTex.transformV(0.5f),

//		0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,	letterSubTex.transformU(0.f),  letterSubTex.transformV(0.5f),
//		 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  letterSubTex.transformU(0.f),  letterSubTex.transformV(1.0f),
//		-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  letterSubTex.transformU(0.33f),letterSubTex.transformV(1.0f),
//		-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  letterSubTex.transformU(0.3f), letterSubTex.transformV(0.5f),

//		-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,  letterSubTex.transformU(0.66f),letterSubTex.transformV(0.5f),
//		-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  letterSubTex.transformU(0.33f),letterSubTex.transformV(0.5f),
//		-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  letterSubTex.transformU(0.33f),letterSubTex.transformV(1.0f),
//		-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,  letterSubTex.transformU(0.66f),letterSubTex.transformV(1.0f),

//		0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,   letterSubTex.transformU(1.0f), letterSubTex.transformV(1.0f),
//		0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,   letterSubTex.transformU(1.0f), letterSubTex.transformV(0.5f),
//		0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,   letterSubTex.transformU(0.66f),letterSubTex.transformV(0.5f),
//		0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,   letterSubTex.transformU(0.66f),letterSubTex.transformV(1.0f)
//};


		/*
		float cubeVertices[8 * 24] = {
			//	 <------ Pos ------>  <--- normal --->  <-- UV -->
				 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,  0.f,
				 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,  0.5f,
				-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f,0.5f,
				-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f,0.f,

				-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33f,0.5f,
				 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f,0.5f,
				 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f,0.f,
				-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33, 0.f,

				-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.f,  0.f,
				 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.66f,0.f,
				 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  0.66f,0.5f,
				-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  1.0f, 0.5f,

				0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,	0.f,  0.5f,
				 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.f,  1.0f,
				-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.33f,1.0f,
				-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.3f, 0.5f,

				-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f,0.5f,
				-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f,0.5f,
				-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f,1.0f,
				-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f,1.0f,

				0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  1.0f,
				0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  0.5f,
				0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 0.5f,
				0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 1.0f
		};



		float pyramidVertices[8 * 16] = {
			// < --- Pos--- >		< ---normal--->			<--UV--> 
			-0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,			0.f, 0.f,
			0.5f, -0.5f, -0.5f,		0.f, -1.f, 0.f,			0.f, 0.5f,
			0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,			0.33f, 0.5f,
			-0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,			0.33f, 0.f,

			-0.5f, -0.5f, -0.5f,	-0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			-0.5f, -0.5f, 0.5f,		-0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			0.0f, 0.5f, 0.0f,		-0.8944f, 0.4472f, 0.f, 0.f, 0.f,

			-0.5f, -0.5f,  0.5f,	0.f, 0.4472f, 0.8944f,	0.f, 0.f,
			 0.5f, -0.5f,  0.5f,	0.f, 0.4472f, 0.8944f,	0.f, 0.f,
			 0.0f,  0.5f,  0.0f,	0.f, 0.4472f, 0.8944f,	0.f, 0.f,

			 0.5f, -0.5f,  0.5f,	0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			 0.5f, -0.5f, -0.5f,	0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			 0.0f,  0.5f,  0.0f,	0.8944f, 0.4472f, 0.f,	0.f, 0.f,

			 0.5f, -0.5f, -0.5f,	0.f, 0.4472f, -0.8944f,	0.f, 0.f,
			-0.5f, -0.5f, -0.5f,	0.f, 0.4472f, -0.8944f, 0.f, 0.f,
			 0.0f,  0.5f,  0.0f,	0.f, 0.4472f, -0.8944f, 0.f, 0.f,

		};

		uint32_t pyramidIndices[3 * 6] =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12,
			13, 14, 15
		};

		uint32_t cubeIndices[3 * 12] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20
		};
		*/
#pragma endregion

#pragma region [History] -  GL_BUFFERS
/*
		std::shared_ptr<VertexArray> cubeVAO;
		std::shared_ptr<VertexBuffer> cubeVBO;
		std::shared_ptr<IndexBuffer> cubeIBO;

		cubeVAO.reset(VertexArray::create());

		VertexBufferLayout cubeBufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
		cubeVBO.reset(VertexBuffer::create(cubeVertices, sizeof(cubeVertices), cubeBufferLayout));

		cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));

		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);


		std::shared_ptr<VertexArray> pyramidVAO;
		std::shared_ptr<VertexBuffer> pyramidVBO;
		std::shared_ptr<IndexBuffer> pyramidIBO;

		pyramidVAO.reset(VertexArray::create());

		pyramidVBO.reset(VertexBuffer::create(pyramidVertices, sizeof(pyramidVertices), cubeBufferLayout));

		pyramidIBO.reset(IndexBuffer::create(pyramidIndices, 18));

		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);
		*/
#pragma endregion
		
#pragma region [History] - SHADERS
/*
		std::shared_ptr<ShaderRend> TPShader;
		TPShader.reset(ShaderRend::create("./assets/shaders/texturedPhong.glsl"));
		*/
#pragma endregion 

#pragma region [History] - MATERIALS
/*
		std::shared_ptr<Material> pyraMat1;
		std::shared_ptr<Material> pyraMat2;
		std::shared_ptr<Material> pyraMat3;
		std::shared_ptr<Material> letterMat;
		std::shared_ptr<Material> numberMat;

		pyraMat1.reset(new Material(TPShader, glm::vec4(1.f, 0.0f, 0.f, 0.5f)));
		pyraMat2.reset(new Material(TPShader, glm::vec4(0.0f, 1.0f, 0.f, 0.7f)));
		pyraMat3.reset(new Material(TPShader, glm::vec4(0.0f, 0.0f, 1.f, 0.2f)));
		letterMat.reset(new Material(TPShader, letterTexture));
		numberMat.reset(new Material(TPShader, numberTexture));

		//Renderer3D::registerShader(TPShader);
		*/
#pragma endregion

#pragma region [History] - MVP
/*
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.f, -6.f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 0.f, -6.f));
		
		models[3] = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 2.f, -6.f));
		models[4] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 2.f, -6.f));
		models[5] = glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 2.f, -6.f));

		//player
		models[6] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -1.f, -3.f));

		//set player follow cam
		/*m_playerCam.reset(new FollowPlayer(models[6]));

		glm::mat4 view = m_camera.getCameraViewMatrix();
		projection =
			glm::perspective(glm::radians(m_camera.getFOV()),(float)RendererShared::SCR_WIDTH  / (float)RendererShared::SCR_HEIGHT, 0.1f, 100.f);

		SceneWideUniforms scenewideUniforms3D;
		scenewideUniforms3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view)));
		scenewideUniforms3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection)));

		glm::mat4 view2D = glm::mat4(1.0f);
		glm::mat4 projection2D = glm::ortho(0.0f, static_cast<float>(m_window->getWidth()), static_cast<float>(m_window->getHeight()), 0.0f);

		SceneWideUniforms scenewideUniforms2D;
		scenewideUniforms2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
		scenewideUniforms2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));
		*/
#pragma endregion

#pragma region [History] - Quad
		/*
		float timestep = 0.f;
		int fps;

		float advance;
		uint32_t xPos = 500.0f;
		Quad quads[5] =
		{
			Quad::createCentreHalfExtens({ m_window->getWidth() / 2 , m_window->getHeight() - 50 }, {m_window->getWidth(),50.0f}),
			Quad::createCentreHalfExtens({ 100, m_window->getHeight() - 50 }, {100,50.0f}),
			Quad::createCentreHalfExtens({ m_window->getWidth() - 100, m_window->getHeight() - 50 }, {100,50.0f}),
			Quad::createCentreHalfExtens({ (m_window->getWidth() / 2) + 350 ,(m_window->getHeight()/2) + 270 }, {100.0f,60.0f}),
			Quad::createCentreHalfExtens({ 200.0f,100 }, {50.0f,50.0f}),
		};
		*/
#pragma endregion 

#pragma region [History] - RenderCommands

		//std::shared_ptr<RendererCommands> clearColorAndDepthCommand;
		//std::shared_ptr<RendererCommands> enableDepthCommand;
		//std::shared_ptr<RendererCommands> enableBlendCommand;
		//std::shared_ptr<RendererCommands> disableDepthCommand;
		//std::shared_ptr<RendererCommands> disableBlendCommand;
		//std::shared_ptr<RendererCommands> blendFuncAlphaCommand;
		//std::shared_ptr<RendererCommands> blendFuncColorCommand;
		//std::shared_ptr<RendererCommands> useTpShaderProgramCommand;

		////clearColorAndDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::clearColorAndDepthBuffer));
		//
		////enclosing in brackets to destroy the pointer as its not needed anytime after
		//{
		//	std::shared_ptr<RendererCommands> setclearCommand;
		//	setclearCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setClearColor, 1.0f, .0f, 1.0f, 1.0f));
		//	RendererShared::actionCommand(setclearCommand);
		//}

		//enableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_DEPTH_TEST));
		//
		//enableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_BLEND));

		//disableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_DEPTH_TEST));
		//
		//disableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_BLEND));
		//
		//blendFuncAlphaCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::blendFuncCommand, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		//blendFuncColorCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::blendFuncCommand, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR));
		
	//	useTpShaderProgramCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::useProgramCommand, TPShader->getRenderID()));

#pragma endregion RenderCommands
		//RendererShared::actionCommand(enableDepthCommand);
		float timestep = 0.f;
		//int fps;

		while (m_running)
		{
#pragma region [History] - While Loop
			/*RendererShared::actionCommand(enableDepthCommand);

			timestep = m_timer->getElapsedTime();
			m_timer->reset();

			if (!isPlayerCam)
				m_camera.update(timestep);

			models[0] = glm::rotate(models[0], timestep, glm::vec3(1.f, 0.0, 0.f));
			models[1] = glm::rotate(models[1], timestep, glm::vec3(0.f, -1.0, 0.0f));
			models[2] = glm::rotate(models[2], timestep, glm::vec3(0.f, 0.0, 1.0f));

			if (isPlayerCam && !autoCam)
			{
				m_cameraSpeed = m_SpeedMultiplier * timestep;
				
				glm::mat4 m = models[6];
				
				if (InputPoller::isKeyPressed(NG_KEY_W))
				{
					Log::info(" W Key Pressed ");
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					glm::vec3 pos = glm::vec3(m[3][0], m[3][1], m[3][2]);
					models[6] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z - m_cameraSpeed));
				}
				if (InputPoller::isKeyPressed(NG_KEY_S))
				{
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					glm::vec3 pos = glm::vec3(m[3][0], m[3][1], m[3][2]);
					models[6] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z + m_cameraSpeed));
				}
				if (InputPoller::isKeyPressed(NG_KEY_A))
				{
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					glm::vec3 pos = glm::vec3(m[3][0], m[3][1], m[3][2]);
					models[6] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x - m_cameraSpeed, pos.y, pos.z));
				}
				if (InputPoller::isKeyPressed(NG_KEY_D))
				{
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					glm::vec3 pos = glm::vec3(m[3][0], m[3][1], m[3][2]);
					models[6] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x + m_cameraSpeed, pos.y, pos.z));
				}
				if (InputPoller::isKeyPressed(NG_KEY_Z))
				{
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					glm::vec3 pos = glm::vec3(m[3][0], m[3][1], m[3][2]);
					models[6] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y + m_cameraSpeed, pos.z));
				}
				if (InputPoller::isKeyPressed(NG_KEY_X))
				{
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					glm::vec3 pos = glm::vec3(m[3][0], m[3][1], m[3][2]);
					models[6] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y - m_cameraSpeed, pos.z));
				}
				if (InputPoller::isKeyPressed(NG_KEY_Q))
				{
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					models[6] = glm::rotate(models[6], m_cameraSpeed, glm::vec3(0.f, 1.0, 0.0f));
				}
				if (InputPoller::isKeyPressed(NG_KEY_E))
				{
					if (m_SpeedMultiplier < 20.0f)
						m_SpeedMultiplier += m_SpeedMultiplierFactor;

					models[6] = glm::rotate(models[6], -m_cameraSpeed, glm::vec3(0.f, 1.0, 0.0f));
				}
			}

			m_playerCam->onUpdate(timestep);

			RendererShared::actionCommand(clearColorAndDepthCommand);
			RendererShared::actionCommand(useTpShaderProgramCommand);

			// camera transformation
			glm::mat4 view = glm::mat4(1.0f);

			if (InputPoller::isKeyPressed(NG_KEY_1))
			{
				autoCam = true;
			}
			if (InputPoller::isKeyPressed(NG_KEY_2))
			{
				autoCam = false;
			}

			if(autoCam)
				view = m_camera.circleCamera();
			else
			{
				if (!isPlayerCam)
					view = m_camera.getCameraViewMatrix();
				else
					view = m_playerCam->getViewMatrix();
			}

			//upload scene uniforms (UBO)
			scenewideUniforms3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view)));
			scenewideUniforms3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection)));

			Renderer3D::begin(scenewideUniforms3D);

			Renderer3D::submit(cubeVAO, letterMat, models[0]);
			Renderer3D::submit(pyramidVAO, pyraMat1, models[1]);
			Renderer3D::submit(cubeVAO, numberMat, models[2]);
			Renderer3D::submit(pyramidVAO, pyraMat2, models[3]);
			Renderer3D::submit(cubeVAO, numberMat, models[4]);
			Renderer3D::submit(pyramidVAO, pyraMat3, models[5]);
			Renderer3D::submit(cubeVAO, letterMat, models[6]);

			Renderer3D::end();

			m_layerStack.Update(timestep);
			m_layerStack.Render();

			//RendererShared::actionCommand(disableDepthCommand);
			//RendererShared::actionCommand(enableBlendCommand);

			//RendererShared::actionCommand(blendFuncAlphaCommand);

			//Renderer2D::begin(scenewideUniforms2D);
			////Renderer2D::submit(quads[0], glm::vec4(1.f, 1.0f, 1.f, 1.f));

			//RendererShared::actionCommand(blendFuncColorCommand);

			//Renderer2D::submit(quads[1], numberSubTex);

			//Renderer2D::flush();

			//RendererShared::actionCommand(blendFuncAlphaCommand);
			//
			//Renderer2D::submit(quads[2], letterSubTex);
			//
			////Renderer2D::submit("FPS :", glm::vec2(20.0f, 50.0f),glm::vec4(1.0f, .0f, .0f, 1.0f));
			//fps = 1.0f / timestep;
			//std::string fps_s = std::to_string(fps);
			//const char* resultFPS = fps_s.c_str();
			//Renderer2D::submit(resultFPS, glm::vec2(100.0f, 50.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			//
			//if(autoCam)
			//	Renderer2D::submit("Rotating Camera", glm::vec2(m_window->getWidth()/2 - 100,m_window->getHeight()- 50),glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			//else
			//	if (isPlayerCam)
			//		Renderer2D::submit("Follow Camera", glm::vec2(m_window->getWidth() / 2 - 75, m_window->getHeight() - 50), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			//	else
			//		Renderer2D::submit("First Person Camera", glm::vec2(m_window->getWidth() / 2 - 100, m_window->getHeight() - 50), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

			//Renderer2D::end();

			RendererShared::actionCommand(disableBlendCommand);
			*/
#pragma endregion
			timestep = m_timer->getElapsedTime();
			m_timer->reset();

			m_physics->m_world->update(timestep);
			m_layerStack.Update(timestep);
			m_layerStack.Render();
			m_window->onUpdate(timestep);
		}

	}
}
