#include "AssimpTestLayer.h"
#include "assimpLoader.h"


AssimpLayer::AssimpLayer(const char* name) : Layer(name)
{
	clearColorAndDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::clearColorAndDepthBuffer));
	std::shared_ptr<RendererCommands> setclearCommand;
	setclearCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setClearColor, 1.0f, 1.0f, 1.0f, 1.0f));
	RendererShared::actionCommand(setclearCommand);

	m_camera.setCameraPos(glm::vec3(0.0f, 0.0f, 3.0f));
	m_view3D = m_camera.getCameraViewMatrix();
	m_projection3D =
		glm::perspective(glm::radians(m_camera.getFOV()), (float)RendererShared::SCR_WIDTH / (float)RendererShared::SCR_HEIGHT, 0.1f, 100.f);

	m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
	m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));




	/*cubeVAO.reset(VertexArray::create());

	VertexBufferLayout cubeBufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
	cubeVBO.reset(VertexBuffer::create(cubeVertices, sizeof(cubeVertices), cubeBufferLayout));

	cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));

	cubeVAO->addVertexBuffer(cubeVBO);
	cubeVAO->setIndexBuffer(cubeIBO);*/


	shader.reset(ShaderRend::create("./assets/shaders/texturedPhong.glsl"));

	Renderer3D::registerShader(shader);

	

	m_model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.f, -6.f));

	Loader::ASSIMPLoad("./assets/models/tank_body.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate, material, cubeVAO, cubeVBO, cubeIBO);

	cubeVAO.reset(VertexArray::create());

	VertexBufferLayout cubeBufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
	cubeVBO.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));

	cubeIBO.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));

	cubeVAO->addVertexBuffer(cubeVBO);
	cubeVAO->setIndexBuffer(cubeIBO);

	material.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(1.0f)));
	//2

	m_model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 2.f, -6.f));

	Loader::ASSIMPLoad("./assets/models/tank_head.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate, material1, cubeVAO2, cubeVBO, cubeIBO2);

	cubeVAO2.reset(VertexArray::create());

	VertexBufferLayout cube1BufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
	cubeVBO.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));

	cubeIBO2.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));

	cubeVAO2->addVertexBuffer(cubeVBO);
	cubeVAO2->setIndexBuffer(cubeIBO2);

	material1.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(1.0f)));
}

void AssimpLayer::OnUpdate(float timestep)
{
	m_camera.update(timestep);
}

void AssimpLayer::OnRender()
{

	RendererShared::actionCommand(clearColorAndDepthCommand);
	m_view3D = m_camera.getCameraViewMatrix();

	m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
	m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));

	Renderer3D::begin(m_swu3D);


	Renderer3D::submit(cubeVAO, material, m_model1);
	Renderer3D::submit(cubeVAO2, material1, m_model2);

	Renderer3D::end();
}

void AssimpLayer::onMouseMoved(MouseMovedEvent& e)
{
	m_camera.mouseMovement(e.getMousePos().x, e.getMousePos().y);
}
