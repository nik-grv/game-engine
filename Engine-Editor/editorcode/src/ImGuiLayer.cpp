#include <glad/glad.h>
#include "../include/ImGuiLayer.h"


//#include "..\OpenGL\ImGuiOpenGL.h"
//#include "..\OpenGL\ImGuiGLFW.h"
//#include "GLFW/glfw3.h
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


namespace Engine {
	static bool dem = true;
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
		ImGuiIO& io = ImGui::GetIO(); 
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::getInstance();
	
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
		ImGui::ShowDemoWindow(&dem);
		ImGui::Render();
		
		glfwMakeContextCurrent((GLFWwindow*)app.getAppWindow()->getNativewindow());
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
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



