#include <glad/glad.h>
#include "../include/ImGuiLayer.h"


//#include "..\OpenGL\ImGuiOpenGL.h"
//#include "..\OpenGL\ImGuiGLFW.h"
//#include "GLFW/glfw3.h"
#include "../editorcode/include/ImGuiHelper.h";


namespace Engine {

	ImGuiLayer::ImGuiLayer(const char* name) : Layer(name)
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		//Log::info("ON ATTACH RUNNING!");
		//IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//ImGuiHelper::init();
		Application& app = Application::getInstance();
		/*
		ImGuiIO& io = ImGui::GetIO();

		//Application& app = Application::getInstance();
		io.DisplaySize = ImVec2(app.getAppWindow()->getWidth(), app.getAppWindow()->getHeight());

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiWindowFlags_NoMove;       // Enable Keyboard Controls

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//Temp - Use Engine KeyCodes?
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
		*/
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.getAppWindow()->getNativewindow(), true);
		ImGui_ImplOpenGL3_Init("#version 440");
	}

	void ImGuiLayer::OnDettach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(float timestep)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Test");
		ImGui::Text("This is window A");
		ImGui::End();
	
		ImGui::Render();
		glfwMakeContextCurrent((GLFWwindow*)app.getAppWindow()->getNativewindow());
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

#pragma region -- [ GUIInputManager ] --
	void ImGuiLayer::onMouseMoved(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.getX(), e.getY());
	}
	void ImGuiLayer::onKeyPressed(KeyPressedEvent& e)
	{
#pragma region Controls
		if (e.getKeyCode() == GLFW_KEY_C)
		{
			glfwSetInputMode(reinterpret_cast<GLFWwindow*>(app.getAppWindow()->getNativewindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if (e.getKeyCode() == NG_KEY_LEFT_ALT)
		{
			glfwSetInputMode(reinterpret_cast<GLFWwindow*>(app.getAppWindow()->getNativewindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
#pragma endregion

		//Log::info("Key Pressed GUI");
	}

	void ImGuiLayer::onKeyReleased(KeyReleasedEvent& e)
	{

	}

	/*void ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e)
	{

	}*/

	void ImGuiLayer::onMouseBtnPressed(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getButton()] = true;

		//Log::info("Mouse Button Pressed GUI");
	}

	void ImGuiLayer::onMouseBtnReleased(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getButton()] = false;
	}

	void ImGuiLayer::onMouseScrolled(MouseScrollEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.getXScroll();
		io.MouseWheel += e.getYScroll();
		//Log::info("Mouse Scrolled GUI");
	}
#pragma endregion

}



