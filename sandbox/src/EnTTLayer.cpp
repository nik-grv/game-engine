#include "EnTTLayer.h"


namespace Engine {


	EnTTLayer::EnTTLayer(const char* name) : Layer(name), m_registry(Application::getInstance().m_registry), m_entities(Application::getInstance().m_entities)
	{
		clearColorAndDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::clearColorAndDepthBuffer));
		setGlLineCmd.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setLineMode));
		setGlFillCmd.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setFillMode));

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
		numberTexture.reset(TextureRend::create("assets/textures/numberCube.png"));
		std::shared_ptr<TextureRend> plainWhiteTex;
		unsigned char whitePixel[4] = { 255,255,255,255 };
		plainWhiteTex.reset(TextureRend::create(1, 1, 4, whitePixel));


		//loading model

		m_camera.setCameraPos(glm::vec3(-1.0f, 1.0f, 6.0f));
		m_view3D = m_camera.getCameraViewMatrix();
		m_projection3D =
			glm::perspective(glm::radians(m_camera.getFOV()), (float)RendererShared::SCR_WIDTH / (float)RendererShared::SCR_HEIGHT, 0.1f, 100.f);

		m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
		m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));

		Renderer3D::init();

		shader.reset(ShaderRend::create("./assets/shaders/texturedPhong.glsl"));
		Renderer3D::registerShader(shader);

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
		mat1.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(1.0f)));

		//2nd model
		Loader::ASSIMPLoad("./assets/models/lettercube/lettercube.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);

		m_VAO2.reset(VertexArray::create());
		VertexBufferLayout cubeBufferLayout2 = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
		m_VBO2.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout2));
		m_IBO2.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_VAO2->addVertexBuffer(m_VBO2);
		m_VAO2->setIndexBuffer(m_IBO2);
		mat2.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(1.0f)));


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

	}

	void EnTTLayer::OnUpdate(float timestep)
	{
		NGPhyiscs::updateTransforms();
		m_camera.update(timestep);

		Log::error("POS - {0},{1},{2}", m_camera.getCameraPos().x, m_camera.getCameraPos().y, m_camera.getCameraPos().z);

	}

	void EnTTLayer::OnRender()
	{
		RendererShared::actionCommand(clearColorAndDepthCommand);

		m_view3D = m_camera.getCameraViewMatrix();

		m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
		m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));


		Renderer3D::begin(m_swu3D);

		auto group = m_registry.group<TransformComponent, RenderComponent>();
		for (auto entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			auto& render = group.get<RenderComponent>(entity);
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
			Renderer3D::submit(m_VAO2, wireframeMat, transformMat);
		}
		Renderer3D::end();

		//set polygon mode to gl_fill back for drawing the models
		RendererShared::actionCommand(setGlFillCmd);
	}

	void EnTTLayer::onMouseMoved(MouseMovedEvent& e)
	{
		m_camera.mouseMovement(e.getMousePos().x, e.getMousePos().y);
	}

	void EnTTLayer::onKeyPressed(KeyPressedEvent& e)
	{
		float rot = 0.25;
		float scale = 0.01;
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

		if (e.getKeyCode() == NG_KEY_SPACE)
		{
			bool found = false;
			int i;
			for (i = 0; i < m_entities.size(); i++)
			{
				if (m_entities[i] == entt::null)
					continue;
			}

			entt::entity projectileEntity = m_registry.create();
			if (i == m_entities.size())
				m_entities.push_back(projectileEntity);
			else
				m_entities[i] = projectileEntity;

			m_registry.emplace<LabelComponent>(projectileEntity, "Projectile");

			auto camTransform = glm::inverse(m_camera.getCameraViewMatrix());

			glm::vec3 forward = { camTransform[2][0],camTransform[2][1] ,camTransform[2][2] };
			glm::vec3 camPos = { camTransform[3][0],camTransform[3][1] ,camTransform[3][2] };
			Log::error("CAMPOS - {0},{1},{2}", camPos.x, camPos.y, camPos.z);
			Log::error("POS - {0},{1},{2}", m_camera.getCameraPos().x, m_camera.getCameraPos().y, m_camera.getCameraPos().z);
			auto projTransform = m_registry.emplace<TransformComponent>(projectileEntity, camPos - forward * 1.0f, glm::vec3(0.0f), glm::vec3(0.5f));

			m_registry.emplace<RenderComponent>(projectileEntity, m_VAO2, mat2);
			m_registry.emplace<RelationshipComponent>(projectileEntity);
			//HierarchySystem::setChild(m_entities[0], projectileEntity);

			auto projectiel_rb = m_registry.emplace<RigidBodyComponent>(projectileEntity, RigidBodyType::Dynamic, projTransform.GetTransform());
			m_registry.emplace<SphereColliderComponent>(projectileEntity, projectiel_rb, 0.5f);
			glm::vec3 force = (-forward * 500.5f);
			projectiel_rb.m_body->applyForceToCenterOfMass(rp3d::Vector3(force.x, force.y + 20.f, force.z + 20.f));

		}
	}
}
