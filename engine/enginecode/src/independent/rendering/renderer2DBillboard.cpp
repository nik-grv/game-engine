#pragma once

#include "rendering/renderer2DBillbord.h"
#include "engine_pch.h"
#include <glad/glad.h>
#include "core/application.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>

namespace Engine {
	VertexBufferLayout BillboardVertex::layout = VertexBufferLayout({ ShaderDataType::Float2, ShaderDataType::Float3, ShaderDataType::Float2, ShaderDataType::FlatInt, /*{ShaderDataType::Byte4, 0, true}*/ShaderDataType::Byte4 }); //Fix Shader Data type for packed byte 4
	
	std::shared_ptr<Renderer2DBillboard::InternalData> Renderer2DBillboard::s_data = nullptr; //wtf is this

	void Renderer2DBillboard::init(uint32_t batchSize) {
		//auto& resources = Application::getInstance().getResourceManager(); // we dont have that yet

		s_data.reset(new InternalData);
		s_data->batchSize = batchSize * 4;

		//check if default texture is there if not, create it. Simon uses default texture of rendererCommon, we dont seem to have that.
		
		//s_data->shader = ShaderRend::create("shader path"); // add shader later or use resource manager

		s_data->UBO.reset(UniformBuffer::create(UniformBufferLayout({
			{"u_view", ShaderDataType::Mat4},
			{"u_projection", ShaderDataType::Mat4}
			})));

		s_data->quad[0] = { -0.5f, -0.5f, 0.f, 1.f };
		s_data->quad[1] = { -0.5f, 0.5f, 0.f, 1.f };
		s_data->quad[2] = { 0.5f, 0.5f, 0.f, 1.f };
		s_data->quad[3] = { 0.5f, -0.5f, 0.f, 1.f };

		s_data->vertices.resize(s_data->batchSize);
		std::vector<uint32_t> indices(s_data->batchSize);
		std::iota(indices.begin(), indices.end(), 0);

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());

	}

	void Renderer2DBillboard::begin(const SceneWideUniforms& swu) {
		s_data->drawCount = 0;

		glUseProgram(s_data->shader->getRenderID());

		/*s_data->shader->uploadIntArray("u_texData", RendererShared::, 32);*/

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->UBO->getRenderID());
	}
}