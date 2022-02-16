#include "../include/EditorLayer.h"

#include "../../sandbox/include/assimpLoader.h"


EditorLayer::EditorLayer(const char* name) : Layer(name)
{
	clearColorAndDepthCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::clearColorAndDepthBuffer));
	std::shared_ptr<RendererCommands> setclearCommand;
	setclearCommand.reset(RenderCommandFactory::createCommand(RendererCommands::Commands::setClearColor, 1.0f, 1.0f, 1.0f, 1.0f));
	RendererShared::actionCommand(setclearCommand);

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

}

void EditorLayer::OnUpdate(float timestep)
{
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), m_rotation.x, { 1.f, 0.f, 0.f });
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), m_rotation.y, { 0.f, 1.f, 0.f });
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), m_rotation.z, { 0.f, 0.f, 1.f });

	glm::mat4 rotation = rotX * rotY * rotZ;

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), { m_scale, m_scale, m_scale });

	m_model1 = rotation * scale; //Code broken here needs fixing !
	//m_model2 = rotation * scale;
	m_model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.f, 0.f, 0.f));

	m_camera.update(timestep);
}

void EditorLayer::OnRender()
{

	RendererShared::actionCommand(clearColorAndDepthCommand);
	m_view3D = m_camera.getCameraViewMatrix();

	m_swu3D["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_view3D)));
	m_swu3D["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(m_projection3D)));

	Renderer3D::begin(m_swu3D);

	//auto count = Loader::s_VAO.size();
	//for (int i = 0; i < count; i++)
	//{
	//	m_VAO = Loader::s_VAO[i];
	//	mat = Loader::s_material[i];
	//	Renderer3D::submit(m_VAO, mat, m_model1);
	//	//m_model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.f, 0.f, 0.f));
	//}

	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), m_rotation.x, { 1.f, 0.f, 0.f });
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), m_rotation.y, { 0.f, 1.f, 0.f });
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), m_rotation.z, { 0.f, 0.f, 1.f });

	glm::mat4 rotation = rotX * rotY * rotZ;

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), { m_scale, m_scale, m_scale });

	m_model1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f)) * rotation * scale;
	Renderer3D::submit(m_VAO1, mat1, m_model1);
	m_model1 = glm::translate(glm::mat4(1.0f), glm::vec3(5.f, 0.f, 0.f)) * rotation * scale;
	Renderer3D::submit(m_VAO1, mat1, m_model1);
	Renderer3D::submit(m_VAO2, mat2, m_model2);


	Renderer3D::end();
}

void EditorLayer::onMouseMoved(MouseMovedEvent& e)
{
	m_camera.mouseMovement(e.getMousePos().x, e.getMousePos().y);
}
