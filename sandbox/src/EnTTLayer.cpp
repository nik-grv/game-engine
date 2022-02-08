#include "EnTTLayer.h"


namespace Engine {


	EnTTLayer::EnTTLayer(const char* name) : Layer(name), m_registry(Application::getInstance().m_registry), m_entities(Application::getInstance().m_entities)
	{
		clearColorAndDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::clearColorAndDepthBuffer));

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

		std::shared_ptr<ShaderRend> shader;
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

		m_entities.resize(5);
		m_entities[0] = m_registry.create();
		m_entities[1] = m_registry.create();
		m_entities[2] = m_registry.create();
		m_entities[3] = m_registry.create();
		m_entities[4] = m_registry.create();
		

		m_registry.emplace<RootComponent>(m_entities[0]);
		
		
		m_registry.emplace<LabelComponent>(m_entities[0],"Root Component");
		m_registry.emplace<LabelComponent>(m_entities[1],"Tank");
		m_registry.emplace<LabelComponent>(m_entities[2],"Cube");
		m_registry.emplace<LabelComponent>(m_entities[3],"Camera");
		m_registry.emplace<LabelComponent>(m_entities[4],"Cube");


		m_registry.emplace<TransformComponent>(m_entities[0]);
		m_registry.emplace<TransformComponent>(m_entities[1],glm::vec3(0),glm::vec3(0),glm::vec3(1));
		m_registry.emplace<TransformComponent>(m_entities[2],glm::vec3(-5,0,0),glm::vec3(0),glm::vec3(1));
		m_registry.emplace<TransformComponent>(m_entities[3],glm::vec3(-1.0f, 1.0f, 6.0f),glm::vec3(0),glm::vec3(1));
		m_registry.emplace<TransformComponent>(m_entities[4], glm::vec3(-3, 0, 0), glm::vec3(0), glm::vec3(1));

		m_registry.emplace<RigidBodyComponent>(m_entities[1]);
	
		
		m_registry.emplace<RenderComponent>(m_entities[1],m_VAO1, mat1);
		m_registry.emplace<RenderComponent>(m_entities[4],m_VAO2, mat1);
		m_registry.emplace<RenderComponent>(m_entities[2], m_VAO2, mat2);
		//m_registry.emplace<RenderComponent>(m_entities[3]);
	
	}


	void EnTTLayer::OnUpdate(float timestep)
	{
		m_camera.update(timestep);
	}

	void EnTTLayer::OnRender()
	{
		RendererShared::actionCommand(clearColorAndDepthCommand);

		Renderer3D::begin(m_swu3D);
		
		auto group = m_registry.group<TransformComponent, RenderComponent>();
		for (auto entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			auto& render = group.get<RenderComponent>(entity);
			Renderer3D::submit(render.m_vao, render.m_mat, transform.GetTransform());
		}

		Renderer3D::end();
	}

	//for loop to render all in render function
}
