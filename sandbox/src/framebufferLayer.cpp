#include "framebufferLayer.h"


namespace Engine {


	FramebufferLayer::FramebufferLayer(const char* name) : Layer(name), m_registry(Application::getInstance().m_registry), m_entities(Application::getInstance().m_entities)
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
		std::shared_ptr<TextureRend> letterTexture;
		letterTexture.reset(TextureRend::create("assets/textures/letterCube.png"));
		std::shared_ptr<TextureRend> numberTexture;
		numberTexture.reset(TextureRend::create("assets/textures/plate.jpg"));
		std::shared_ptr<TextureRend> plainWhiteTex;
		unsigned char whitePixel[4] = { 255,255,255,255 };
		plainWhiteTex.reset(TextureRend::create(1, 1, 4, whitePixel));


		//loading model
		
		Renderer3D::init();

		shader.reset(ShaderRend::create("./assets/shaders/texturedPhong.glsl"));
		Renderer3D::registerShader(shader);

		//1st model
		//Loader::ASSIMPLoad("./assets/models/zard/mesh.3DS", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);
		//Loader::ASSIMPLoad("./assets/models/bob/boblampclean.md5mesh", aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace );
		VertexBufferLayout cubeBufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };


		Loader::ASSIMPLoad("./assets/models/Tank/TankBody.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);
		m_VAO1.reset(VertexArray::create());
		m_VBO1.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));
		m_IBO1.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_VAO1->addVertexBuffer(m_VBO1);
		m_VAO1->setIndexBuffer(m_IBO1);


		Loader::ASSIMPLoad("./assets/models/Tank/TankHead.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);
		m_VAO2.reset(VertexArray::create());
		m_VBO2.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));
		m_IBO2.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_VAO2->addVertexBuffer(m_VBO2);
		m_VAO2->setIndexBuffer(m_IBO2);

		Loader::ASSIMPLoad("./assets/models/Tank/TankBarrel.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);
		m_VAO3.reset(VertexArray::create());
		m_VBO3.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));
		m_IBO3.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_VAO3->addVertexBuffer(m_VBO3);
		m_VAO3->setIndexBuffer(m_IBO3);

		mat1.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(1.0f)));

		//2nd model
		Loader::ASSIMPLoad("./assets/models/lettercube/lettercube.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);
		m_VAO4.reset(VertexArray::create());
		m_VBO4.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));
		m_IBO4.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_VAO4->addVertexBuffer(m_VBO4);
		m_VAO4->setIndexBuffer(m_IBO4);
		mat2.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(1.0f)));

		plateMat.reset(new Material(shader, numberTexture));
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

		m_entities.resize(8);
		m_entities[0] = m_registry.create();
		m_entities[1] = m_registry.create();
		m_entities[2] = m_registry.create();
		m_entities[3] = m_registry.create();
		m_entities[4] = m_registry.create();
		m_entities[5] = m_registry.create();
		m_entities[6] = m_registry.create();
		m_entities[7] = m_registry.create();


		m_registry.emplace<RootComponent>(m_entities[0]);


		m_registry.emplace<LabelComponent>(m_entities[0], "Root Component");
		m_registry.emplace<LabelComponent>(m_entities[1], "TankBody");
		m_registry.emplace<LabelComponent>(m_entities[2], "Cube");
		m_registry.emplace<LabelComponent>(m_entities[3], "Camera");
		m_registry.emplace<LabelComponent>(m_entities[4], "Floor");
		m_registry.emplace<LabelComponent>(m_entities[5], "FirePoint");
		m_registry.emplace<LabelComponent>(m_entities[6], "TankHead");
		m_registry.emplace<LabelComponent>(m_entities[7], "TankBarrel");
		//m_registry.emplace<LabelComponent>(m_entities[5], "Slope");


		m_registry.emplace<TransformComponent>(m_entities[0],glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0),glm::vec3(1));//ROOT
		m_registry.emplace<TransformComponent>(m_entities[1], glm::vec3(2.0f, 2.f, 0.0f), glm::vec3(0), glm::vec3(1));//TANK BODY
		m_registry.emplace<TransformComponent>(m_entities[2], glm::vec3(10.0f, 20.f, 0), glm::vec3(0), glm::vec3(1));//CUBE
		m_registry.emplace<TransformComponent>(m_entities[3], glm::vec3(-1.0f, 1.0f, 6.0f), glm::vec3(0), glm::vec3(1));//CAMERA (NOT IN USE)
		m_registry.emplace<TransformComponent>(m_entities[4], glm::vec3(0, 0.f, 0), glm::vec3(0), glm::vec3(30.0f, 1.f, 30.0f));//FLOOR
		m_registry.emplace<TransformComponent>(m_entities[5], glm::vec3(0, 1.525f, -2.3f), glm::vec3(0), glm::vec3(0.125f));//FIRE POINT
		m_registry.emplace<TransformComponent>(m_entities[6], glm::vec3(0.0f, 0.8f, 1.0f), glm::vec3(0), glm::vec3(1));//TANK HEAD
		m_registry.emplace<TransformComponent>(m_entities[7], glm::vec3(0.0f, 1.25f, -1.0f), glm::vec3(0), glm::vec3(1));//TANK BARREL
		//m_registry.emplace<TransformComponent>(m_entities[5]);

		
		auto& tankTransform = m_registry.get<TransformComponent>(m_entities[1]).GetTransform();
		auto& cubeTransform = m_registry.get<TransformComponent>(m_entities[2]).GetTransform();
		auto& floorTransform = m_registry.get<TransformComponent>(m_entities[4]).GetTransform();
		auto& firePtTransform = m_registry.get<TransformComponent>(m_entities[5]).GetTransform();
		auto& tankHeadTransform = m_registry.get<TransformComponent>(m_entities[6]).GetTransform();
		auto& tankBarrelTransform = m_registry.get<TransformComponent>(m_entities[7]).GetTransform();
		//auto slopeTransform = m_registry.get<TransformComponent>(m_entities[5]).GetTransform();

		auto tank_rb = m_registry.emplace<RigidBodyComponent>(m_entities[1], RigidBodyType::Dynamic, tankTransform);
		auto cube_rb = m_registry.emplace<RigidBodyComponent>(m_entities[2], RigidBodyType::Dynamic, cubeTransform);
		auto floor_rb = m_registry.emplace<RigidBodyComponent>(m_entities[4], RigidBodyType::Static, floorTransform);
		auto tankHead_rb = m_registry.emplace<RigidBodyComponent>(m_entities[6], RigidBodyType::Kinematic, tankHeadTransform);
		auto tankBarrel_rb = m_registry.emplace<RigidBodyComponent>(m_entities[7], RigidBodyType::Kinematic, tankBarrelTransform);
		auto fire_rb = m_registry.emplace<RigidBodyComponent>(m_entities[5], RigidBodyType::Kinematic, firePtTransform);
		m_tankRB = tank_rb;

		//auto slope_rb = m_registry.emplace<RigidBodyComponent>(m_entities[5], RigidBodyType::Static, slopeTransform);

		m_registry.emplace<BoxColliderComponent>(m_entities[1], tank_rb, glm::vec3(2.72f, 1.15f, 5.f) * 0.5f);
		m_registry.emplace<BoxColliderComponent>(m_entities[2], cube_rb, glm::vec3(1.0f, 1.f, 1.0f) * 0.5f);
		m_registry.emplace<BoxColliderComponent>(m_entities[4], floor_rb, glm::vec3(30.0f, 1.f,30.0f) * 0.5f);
		//m_registry.emplace<BoxColliderComponent>(m_entities[6], tankHead_rb, glm::vec3(2.0f, 0.5f,2.9f) * 0.5f);
		//auto something = m_registry.emplace<HeightmapCollider>(m_entities[5], slope_rb, 7, 7, -3, 3, heigthData);

		m_registry.emplace<RenderComponent>(m_entities[1], m_VAO1, mat1);
		m_registry.emplace<RenderComponent>(m_entities[2], m_VAO4, mat2);
		m_registry.emplace<RenderComponent>(m_entities[4], m_VAO4, plateMat);
		m_registry.emplace<RenderComponent>(m_entities[6], m_VAO2, mat1);
		m_registry.emplace<RenderComponent>(m_entities[7], m_VAO3, mat1);
		m_registry.emplace<RenderComponent>(m_entities[5], m_VAO4, mat2);
		
		m_registry.emplace<RelationshipComponent>(m_entities[1]);
		m_registry.emplace<RelationshipComponent>(m_entities[5]);
		m_registry.emplace<RelationshipComponent>(m_entities[6]);
		m_registry.emplace<RelationshipComponent>(m_entities[7]);


		//PARENTING....
		HierarchySystem::setChild(m_entities[1], m_entities[6]); //TANK IS PARENT --> TANK HEAD IS CHILD
		HierarchySystem::setChild(m_entities[1], m_entities[7]); //TANK IS PARENT --> TANK BARREL IS CHILD
		HierarchySystem::setChild(m_entities[1], m_entities[5]); //TANK IS PARENT --> TANK FIRE POINT IS CHILD
		HierarchySystem::setChild(m_entities[6], m_entities[5]); //TANK HEAD IS PARENT --> TANK BARREL IS CHILD
		HierarchySystem::setChild(m_entities[7], m_entities[5]); //TANK HEAD IS PARENT --> TANK FIRE POINT IS CHILD
		//HierarchySystem::setChild(m_entities[7], m_entities[5]); //TANK BARREL IS PARENT --> TANK FIRE POINT IS CHILD

		//set tank script
		auto& script = m_registry.emplace<NativeScriptComponent>(m_entities[1]);
		script.create<TankController>(m_entities[1], 5.f, true);


		//set player cam

		m_camera.setCameraPos(glm::vec3(-1.0f, 1.0f, 6.0f));

		auto& transform = m_registry.get<TransformComponent>(m_entities[0]);
		m_followCam.reset(new FollowPlayer(transform.GetTransform()));

		if (!m_isPlayerCam)
		{
			m_view3D = m_camera.getCameraViewMatrix();
		}
		else
		{
			glm::mat4 view = glm::mat4(1.0f);
			m_view3D = m_followCam->getViewMatrix();
		}

		m_projection3D =
			glm::perspective(glm::radians(m_camera.getFOV()), (float)RendererShared::SCR_WIDTH / (float)RendererShared::SCR_HEIGHT, 0.1f, 100.f);

		m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
		m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));


	

		//framebuffer stuff...

		FramebufferLayout fbLayout = { {AttachmentType::Color,true},{AttachmentType::Depth,false }  };

		textureTarget.reset(Framebuffer::create(glm::ivec2(window->getWidth(), window->getHeight()), fbLayout));
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

	
	void FramebufferLayer::OnUpdate(float timestep)
	{
		ScriptSystem::UpdateScripts(timestep);
		NGPhyiscs::updateTransforms();
		if (m_isPlayerCam)
			m_followCam->onUpdate(timestep);
		else
			m_camera.update(timestep);
		HierarchySystem::UpdateChildren();
	}

	void FramebufferLayer::OnRender()
	{	
		textureTarget->use();

		//render 3d scene...

		RendererShared::actionCommand(clearColorAndDepthCommand);
		RendererShared::actionCommand(enableDepthCommand);
		RendererShared::actionCommand(disableBlendCommand);

		if (!m_isPlayerCam)
			m_view3D = m_camera.getCameraViewMatrix();
		else
			m_view3D = m_followCam->getViewMatrix();


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
		wireframeMat.reset(new Material(shader, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));

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
			Renderer3D::submit(m_VAO4, wireframeMat, transformMat);
		}
		Renderer3D::end();

		//set polygon mode to gl_fill back for drawing the models
		RendererShared::actionCommand(setGlFillCmd);

		//render the framebuffer on a 2d quad

		defaultTarget->use();

		RendererShared::actionCommand(disableDepthCommand);
		

		Renderer2D::begin(m_swu2D);
		Renderer2D::submit(m_screenQuad, m_screenTexture);
		Renderer2D::end();

		RendererShared::actionCommand(enableBlendCommand);
		RendererShared::actionCommand(standardBlend);

		Renderer2D::begin(m_swu2D);
		if(m_isPlayerCam)
		Renderer2D::submit("Player cam", glm::vec2(500, 500), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		else
			Renderer2D::submit("Free cam", glm::vec2(500, 500), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Renderer2D::end();

	}

	void FramebufferLayer::onMouseMoved(MouseMovedEvent& e)
	{
		ScriptSystem::OnMouseMoved(e);
		//	
		//}
		//else
		//{
		if(!m_isPlayerCam)
			m_camera.mouseMovement(e.getMousePos().x, e.getMousePos().y);
		//}
	}

	void FramebufferLayer::onMouseBtnPressed(MouseButtonPressedEvent& e)
	{
		ScriptSystem::OnMouseBtnPressed(e);
		
	}
	

	void FramebufferLayer::onKeyPressed(KeyPressedEvent& e)
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

		ScriptSystem::OnKeyPressed(e);
		/*if (e.getKeyCode() == NG_KEY_KP_ADD)
		{
			if (InputPoller::isKeyPressed(NG_KEY_X)) { m_rotation.x += rot; Log::info("X pressed"); }
			else if (InputPoller::isKeyPressed(NG_KEY_Y)) { m_rotation.y += rot; }
			else if (InputPoller::isKeyPressed(NG_KEY_Z)) { m_rotation.z += rot; }
			else { m_scale += scale; }
		}

		if (e.getKeyCode() == NG_KEY_KP_SUBTRACT)
		{
			if (InputPoller::isKeyPressed(NG_KEY_X)) { m_rotation.x -= rot; }
			else if (InputPoller::isKeyPressed(NG_KEY_Y)) { m_rotation.y -= rot; }
			else if (InputPoller::isKeyPressed(NG_KEY_Z)) { m_rotation.z -= rot; }
			else if (m_scale > scale) { m_scale -= scale; }
		}*/


		////switch camera type
		if (e.getKeyCode() == NG_KEY_SPACE)
		{
			m_isPlayerCam = !m_isPlayerCam;
		}


		//if (e.getKeyCode() == NG_KEY_UP)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(0.0f, 0.0f, -25.0f));
		//}
		//if (e.getKeyCode() == NG_KEY_DOWN)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(0.0f, 0.0f, 25.0f));
		//}
		//if (e.getKeyCode() == NG_KEY_LEFT)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(-25.0f, 0.0f, 0.0f));
		//}
		//if (e.getKeyCode() == NG_KEY_RIGHT)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(25.0f, 0.0f, 0.0f));
		//}


	}
}
