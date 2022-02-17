/** \file SceneLayer.h*/
#pragma once


#include <rendering/Renderer3D.h>

#include <rendering/shaderDataType.h>
#include "engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera/Camera.h"


using namespace Engine;

class EditorLayer : public Layer
{
public:
	EditorLayer(const char* name);
	void OnUpdate(float timestep) override;
	void OnRender() override;
	void onMouseMoved(MouseMovedEvent& e) override;
private:

	std::shared_ptr<RendererCommands> clearColorAndDepthCommand;


#pragma region RAW_DATA

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
#pragma endregion




	std::shared_ptr<RendererCommands> enableBlendCommand;
	std::shared_ptr<RendererCommands> disableDepthCommand;
	std::shared_ptr<RendererCommands> blendFuncAlphaCommand;

	std::shared_ptr<VertexArray> cubeVAO;
	std::shared_ptr<VertexBuffer> cubeVBO;
	std::shared_ptr<IndexBuffer> cubeIBO;


	std::shared_ptr<VertexArray> m_VAO1, m_VAO2;
	std::shared_ptr<VertexBuffer> m_VBO1, m_VBO2;
	std::shared_ptr<IndexBuffer> m_IBO1, m_IBO2;
	std::shared_ptr<ShaderRend> shader;
	std::shared_ptr<Material> mat1, mat2;

	
	std::shared_ptr<ShaderRend> TPShader;
	std::shared_ptr<Material> letterMat;


	Camera m_camera;
	glm::mat4 m_view3D;
	glm::mat4 m_projection3D;
	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>; //!< scene wide uniform 
	SceneWideUniforms m_swu3D;

	glm::mat4 m_model1, m_model2;

	glm::vec3 m_rotation = { 0.f, 0.f, 0.f };
	float m_scale = 1.f;




};
