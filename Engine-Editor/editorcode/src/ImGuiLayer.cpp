#include "engine_pch.h"
#include <glad/glad.h>
#include "../include/ImGuiLayer.h"
#include <Utilities/PlatformUtils.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <assimpLoader.h>
//#include "../editorcode/include/ImGuiHelper.h";

namespace Engine {

	ImGuiLayer::ImGuiLayer(const char* name) : Layer(name), m_registry(Application::getInstance().m_registry), m_entities(Application::getInstance().m_entities)
	{
		clearColorAndDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::clearColorAndDepthBuffer));
		setGlLineCmd.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setLineMode));
		setGlFillCmd.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setFillMode));

		enableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_BLEND));
		disableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_DEPTH_TEST));
		enableDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::enableCommand, GL_DEPTH_TEST));
		disableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_BLEND));
		disableBlendCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::disableCommand, GL_BLEND));
		standardBlend.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::blendFuncCommand, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		auto& window = Application::getInstance().getAppWindow();
		{
			std::shared_ptr<RendererCommands> setclearCommand;
			setclearCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setClearColor, 1.0f, 1.0f, 1.0f, 1.0f));
			RendererShared::actionCommand(setclearCommand);
		}

		//textures
		textureMap[std::string("letterTexture")] = std::shared_ptr<TextureRend>(TextureRend::create("assets/textures/letterCube.png"));
		textureMap[std::string("numberTexture")] = std::shared_ptr<TextureRend>(TextureRend::create("assets/textures/numberCube.png"));
		textureMap[std::string("plainWhiteTex")] = std::shared_ptr<TextureRend>(TextureRend::create("assets/textures/plainWhiteTex.png"));
		/*std::shared_ptr<TextureRend> letterTexture;
		letterTexture.reset(TextureRend::create("assets/textures/letterCube.png"));
		std::shared_ptr<TextureRend> numberTexture;
		numberTexture.reset(TextureRend::create("assets/textures/numberCube.png"));
		std::shared_ptr<TextureRend> plainWhiteTex;
		unsigned char whitePixel[4] = { 255,255,255,255 };
		plainWhiteTex.reset(TextureRend::create(1, 1, 4, whitePixel));
		*/

		//set the 2d camera
		m_view2D = glm::mat4(1.0f);
		m_projection2D = glm::ortho(0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.0f);

		m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view2D)));
		m_swu2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection2D)));

		m_screenQuad = Quad::createCentreHalfExtens(glm::vec2(RendererShared::SCR_WIDTH * 0.5f, RendererShared::SCR_HEIGHT * 0.5f),
			glm::vec2(RendererShared::SCR_WIDTH * 0.5f, RendererShared::SCR_HEIGHT * 0.5f));

		//loading model
		m_camera.setCameraPos(glm::vec3(-1.0f, 1.0f, 6.0f));
		m_view3D = m_camera.getCameraViewMatrix();
		m_projection3D =
			glm::perspective(glm::radians(m_camera.getFOV()), (float)RendererShared::SCR_WIDTH / (float)RendererShared::SCR_HEIGHT, 0.1f, 100.f);

		m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
		m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));

		Renderer3D::init();

		//shader
		shaderMap[std::string("texturedPhong")] = std::shared_ptr<ShaderRend>(ShaderRend::create("./assets/shaders/texturedPhong.glsl"));
		//shader.reset(ShaderRend::create("./assets/shaders/texturedPhong.glsl"));
		Renderer3D::registerShader(shaderMap["texturedPhong"]);

		//1st model
		//Loader::ASSIMPLoad("./assets/models/zard/mesh.3DS", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);
		//Loader::ASSIMPLoad("./assets/models/bob/boblampclean.md5mesh", aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace );
		Loader::ASSIMPLoad("./assets/models/Tank.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);

		m_VAO1.reset(VertexArray::create());
		VertexBufferLayout cubeBufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
		m_VBO1.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));
		m_IBO1.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_VAO1->addVertexBuffer(m_VBO1);
		m_VAO1->setIndexBuffer(m_IBO1);
		mat1.reset(new Material(shaderMap["texturedPhong"], Loader::output.diffusTex, glm::vec4(1.0f)));

		//2nd model
		Loader::ASSIMPLoad("./assets/models/lettercube/lettercube.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);

		m_VAO2.reset(VertexArray::create());
		VertexBufferLayout cubeBufferLayout2 = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
		m_VBO2.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout2));
		m_IBO2.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_VAO2->addVertexBuffer(m_VBO2);
		m_VAO2->setIndexBuffer(m_IBO2);
		mat2.reset(new Material(shaderMap["texturedPhong"], Loader::output.diffusTex, glm::vec4(1.0f)));


		// height data used for a height map later on
		std::vector<float> heigthData;
		heigthData.resize(49);
		for (size_t i = 0; i < 7; i++)	//size_t is an unsigned int
		{
			heigthData.at(i) = i - 3;
			heigthData.at(i + 7) = i - 3;
			heigthData.at(i + 14) = i - 3;
			heigthData.at(i + 21) = i - 3;
			heigthData.at(i + 28) = i - 3;
			heigthData.at(i + 35) = i - 3;
			heigthData.at(i + 42) = i - 3;

		}

		m_entities.resize(5);
		m_entities[0] = m_registry.create();
		m_entities[1] = m_registry.create();
		m_entities[2] = m_registry.create();
		m_entities[3] = m_registry.create();
		m_entities[4] = m_registry.create();
		//m_entities[5] = m_registry.create();


		m_registry.emplace<RootComponent>(m_entities[0]);


		m_registry.emplace<LabelComponent>(m_entities[0], "Root Component");
		m_registry.emplace<LabelComponent>(m_entities[1], "Tank");
		m_registry.emplace<LabelComponent>(m_entities[2], "Cube");
		m_registry.emplace<LabelComponent>(m_entities[3], "Camera");
		m_registry.emplace<LabelComponent>(m_entities[4], "Floor");
		//m_registry.emplace<LabelComponent>(m_entities[5], "Slope");


		m_registry.emplace<TransformComponent>(m_entities[0]);
		m_registry.emplace<TransformComponent>(m_entities[1], glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0), glm::vec3(1));
		m_registry.emplace<TransformComponent>(m_entities[2], glm::vec3(0.0f, 20, 0), glm::vec3(0), glm::vec3(1));
		m_registry.emplace<TransformComponent>(m_entities[3], glm::vec3(-1.0f, 1.0f, 6.0f), glm::vec3(0), glm::vec3(1));
		m_registry.emplace<TransformComponent>(m_entities[4], glm::vec3(0, 0.f, 0), glm::vec3(0), glm::vec3(15.0f, 1.f, 15.0f));
		//m_registry.emplace<TransformComponent>(m_entities[5]);

		auto tankTransform = m_registry.get<TransformComponent>(m_entities[1]).GetTransform();
		auto cubeTransform = m_registry.get<TransformComponent>(m_entities[2]).GetTransform();
		auto floorTransform = m_registry.get<TransformComponent>(m_entities[4]).GetTransform();
		//auto slopeTransform = m_registry.get<TransformComponent>(m_entities[5]).GetTransform();

		auto tank_rb = m_registry.emplace<RigidBodyComponent>(m_entities[1], RigidBodyType::Dynamic, tankTransform);
		auto cube_rb = m_registry.emplace<RigidBodyComponent>(m_entities[2], RigidBodyType::Dynamic, cubeTransform);
		auto floor_rb = m_registry.emplace<RigidBodyComponent>(m_entities[4], RigidBodyType::Static, floorTransform);
		//auto slope_rb = m_registry.emplace<RigidBodyComponent>(m_entities[5], RigidBodyType::Static, slopeTransform);

		m_registry.emplace<BoxColliderComponent>(m_entities[1], tank_rb, glm::vec3(2.72f, 1.22f, 5.f) * 0.5f);
		m_registry.emplace<BoxColliderComponent>(m_entities[2], cube_rb, glm::vec3(1.0f, 1.f, 1.0f) * 0.5f);
		m_registry.emplace<BoxColliderComponent>(m_entities[4], floor_rb, glm::vec3(15.0f, 1.f, 15.0f) * 0.5f);
		//auto something = m_registry.emplace<HeightmapCollider>(m_entities[5], slope_rb, 7, 7, -3, 3, heigthData);
		//Log::error("{0}", something.collider.)

		m_registry.emplace<RenderComponent>(m_entities[1], m_VAO1, mat1);
		m_registry.emplace<RenderComponent>(m_entities[2], m_VAO2, mat2);
		m_registry.emplace<RenderComponent>(m_entities[4], m_VAO2, mat1);


		//framebuffer stuff...

		FramebufferLayout fbLayout = { {AttachmentType::Color,true},{AttachmentType::Depth,false } };

		textureTarget.reset(Framebuffer::create(glm::ivec2((window->getWidth()), (window->getHeight())), fbLayout));
		defaultTarget.reset(Framebuffer::createDefault());

		//set the 2d camera
		m_view2D = glm::mat4(1.0f);
		m_projection2D = glm::ortho(0.0f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.0f);

		m_swu2D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view2D)));
		m_swu2D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection2D)));

		m_screenQuad = Quad::createCentreHalfExtens(glm::vec2(RendererShared::SCR_WIDTH * 0.5f, RendererShared::SCR_HEIGHT * 0.5f),
			glm::vec2(RendererShared::SCR_WIDTH * 0.5f, RendererShared::SCR_HEIGHT * 0.5f));

		m_screenTexture = SubTexture(textureTarget->getTexture(0), glm::vec2(0, 1), glm::vec2(1, 0));
	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewPortsNoTaskIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		
		ImGui::StyleColorsDark();


		m_IconPlay.reset(TextureRend::create("Resources/Icons/PlayButton.png"));
		m_IconStop.reset(TextureRend::create("Resources/Icons/StopButton.png"));
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.getAppWindow()->getNativewindow(), true);
		ImGui_ImplOpenGL3_Init("#version 440");
		Log::e_info("== Application Started ==");
		Log::e_info("");

		//Current Work in Progress

		//auto tank = m_ActiveScene->CreateEntity();
		//m_ActiveScene->Reg().emplace<TransformComponent>(tank);
		//m_ActiveScene->Reg().emplace<SpriteRenderComponent>(tank, glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

		
		
		
	}

	void ImGuiLayer::OnDettach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(float timestep)
	{

		NGPhyiscs::updateTransforms();
		m_camera.update(timestep);
		m_ActiveScene->OnUpdate(timestep);
	}

	void ImGuiLayer::OnRender()
	{

		textureTarget->use();

		//render 3d scene...

		RendererShared::actionCommand(clearColorAndDepthCommand);
		RendererShared::actionCommand(enableDepthCommand);
		RendererShared::actionCommand(disableBlendCommand);

		m_view3D = m_camera.getCameraViewMatrix();

		m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
		m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));


		Renderer3D::begin(m_swu3D);

		auto group = m_registry.group<TransformComponent, RenderComponent>();
		for (auto entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			auto& render = group.get<RenderComponent>(entity);
			if (usePP)
			{
				render.m_mat->getShader()->uploadInt("setPP", 1);
			}
			else
			{
				render.m_mat->getShader()->uploadInt("setPP", 0);
			}
			Renderer3D::submit(render.m_vao, render.m_mat, transform.GetTransform());
		}
		Renderer3D::end();

		//draw wireframe of box colliders
		Renderer3D::begin(m_swu3D);

		//set polygon mode to gl_Line
		RendererShared::actionCommand(setGlLineCmd);
		wireframeMat.reset(new Material(shaderMap["texturedPhong"], glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));

		auto bxColl = m_registry.view<TransformComponent, BoxColliderComponent>();
		for (auto entity : bxColl)
		{
			auto& transform = bxColl.get<TransformComponent>(entity);
			auto& collider = bxColl.get<BoxColliderComponent>(entity);
			//auto& [transform, collider] = bxColl.get<TransformComponent, BoxColliderComponent>(entity);

			glm::vec3 t = transform.translation;
			glm::quat r = transform.rotation;

			rp3d::Vector3 s = collider.shape->getHalfExtents() * 2;
			glm::vec3 scale = glm::vec3(s.x, s.y, s.z);

			glm::mat4 transformMat = glm::translate(glm::mat4(1.f), t) * glm::toMat4(r) * glm::scale(glm::mat4(1.f), scale);
			Renderer3D::submit(m_VAO2, wireframeMat, transformMat);
		}
		Renderer3D::end();

		//set polygon mode to gl_fill back for drawing the models
		RendererShared::actionCommand(setGlFillCmd);

		//render the framebuffer on a 2d quad
		
		//auto& qd = Quad::createCentreHalfExtens(glm::vec2(2.f, 2.f), glm::vec2(1.f, 1.f));
		//Renderer2D::begin(m_swu2D);
		//Renderer2D::submit();



		Begin();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O")) 
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit"));
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		UI_Toolbar();

		m_ContentBrowserPanel.OnImGuiRender();
		m_SceneHierarchyPanel.OnImGuiRender();

		defaultTarget->use();
		ImGui::Begin("Visualizer");
		uint32_t textureID = textureTarget->getTexture(0)->getRenderID();
		ImGui::Image((void*)textureID, ImVec2{ 800, 600 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		End();

	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
		auto& window = Engine::Application::getInstance().getAppWindow();
		io.DisplaySize = ImVec2(window->getWidth(), window->getHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		std::shared_ptr<TextureRend> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->getRenderID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
    
	

	}

#pragma region -- [ GUIInputManager ] --
	void ImGuiLayer::onMouseMoved(MouseMovedEvent& e)
	{
		if (m_ViewportFocused)
			cameraUpdate(e);
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

		if (m_ViewportFocused)
			moveUpdate(e);
#pragma endregion

	}

	void ImGuiLayer::cameraUpdate(MouseMovedEvent e)
	{
		m_camera.mouseMovement(e.getMousePos().x, e.getMousePos().y);
	}

	void ImGuiLayer::moveUpdate(KeyPressedEvent e)
	{
		float rot = 0.25;
		float scale = 0.01;

		if (e.getKeyCode() == NG_KEY_0)
		{
			usePP = true;
		}
		if (e.getKeyCode() == NG_KEY_1)
		{
			usePP = false;
		}

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
	}
#pragma endregion

	void ImGuiLayer::NewScene()
	{
		Log::e_debug("== Creating Scene File ==");

	}

	void ImGuiLayer::OpenScene()
	{
		Log::e_debug("== Opening File ==");
	}

	void ImGuiLayer::OpenScene(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".tanky")
		{
			Log::e_error("Could not load {0} -not a scene file", path.filename().string());
		}
	}

	void ImGuiLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			Log::e_debug("== Saving File ==");
		}
	}

	void ImGuiLayer::Exit()
	{
		Log::e_debug("== Exiting Editor ==");
	
	}

	void ImGuiLayer::OnScenePlay()
	{
		Log::e_debug("== Scene Started ==");
		m_SceneState = SceneState::Play;
	}

	void ImGuiLayer::OnSceneStop()
	{
		Log::e_debug("== Scene Stopped ==");
		m_SceneState = SceneState::Edit;

	}

}



