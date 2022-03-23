#pragma once

#include "engine_pch.h"
#include "rendering/renderer2DBillbord.h"
#include <glad/glad.h>
#include "core/application.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>

namespace Engine {
	VertexBufferLayout BillboardVertex::layout = VertexBufferLayout({ ShaderDataType::Float2, ShaderDataType::Float3, ShaderDataType::Float2, ShaderDataType::FlatInt, /*{ShaderDataType::Byte4, 0, true}*/ShaderDataType::Byte4}); //Fix Shader Data type for packed byte 4
	
	std::shared_ptr<Renderer2DBillboard::InternalData> Renderer2DBillboard::s_data = nullptr; //wtf is this

	void Renderer2DBillboard::init(uint32_t batchSize) {
		//auto& resources = Application::getInstance().getResourceManager(); // we dont have that yet

		s_data.reset(new InternalData);
		s_data->batchSize = batchSize * 4;

		//check if default texture is there if not, create it. Simon uses default texture of rendererCommon, we dont seem to have that.
		

		unsigned char whitePixel[4] = { 255,255,255,255 };
		s_data->defTexture.reset(TextureRend::create(1, 1, 4, whitePixel));

		s_data->defaultSubTexture = SubTexture(s_data->defTexture, glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f));

		s_data->shader.reset(ShaderRend::create("./assets/shaders/quadBillboard.glsl")); // add shader later or use resource manager

		s_data->UBO.reset(UniformBuffer::create(UniformBufferLayout({
			{"u_view", ShaderDataType::Mat4},
			{"u_projection", ShaderDataType::Mat4}
			})));

		for (int i = 0; i < s_data->texUnits.size(); i++)
		{
			s_data->texUnits[i] = i;
		}


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
		VBO.reset(VertexBuffer::create(s_data->vertices.data(), sizeof(BillboardVertex) * s_data->vertices.size(), BillboardVertex::layout));
		IBO.reset(IndexBuffer::create(indices.data(), indices.size()));
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);

		s_data->UBO->attachShaderBlock(s_data->shader, "b_camera");
	}

	void Renderer2DBillboard::begin(const SceneWideUniforms& swu) {
		s_data->drawCount = 0;

		glUseProgram(s_data->shader->getRenderID());

		s_data->shader->uploadIntArray("u_texData", s_data->texUnits.data(), 32);

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->UBO->getRenderID());
		s_data->UBO->uploadDataToUB("u_projection", swu.at("u_projection").second);
		s_data->UBO->uploadDataToUB("u_view", swu.at("u_view").second);
	}

	void Renderer2DBillboard::submit(const BillboardQuad& quad, const glm::vec4& tint)
	{
		Renderer2DBillboard::submit(quad, tint, s_data->defaultSubTexture);
	}

	void Renderer2DBillboard::submit(const BillboardQuad& quad, const glm::vec4& tint, const SubTexture& texture) {
		if (s_data->drawCount + 4 > s_data->batchSize) flush();

		uint32_t packedTint = RendererShared::pack(tint);

		uint32_t textureUnit;
		if (RendererShared::s_tum.full()) flush();
		if (RendererShared::s_tum.getUnit(texture.getBaseTexture()->getRenderID(), textureUnit)) {
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, texture.getBaseTexture()->getRenderID());
		}

		uint32_t startIndex = s_data->drawCount;
		for (int i = 0; i < 4; i++) {
			s_data->vertices[i + startIndex].position = glm::vec2(s_data->quad[i].x * quad.m_size.x, s_data->quad[i].y * quad.m_size.y);
			s_data->vertices[i + startIndex].center = quad.m_center;
			s_data->vertices[i + startIndex].tint = packedTint;
			s_data->vertices[i + startIndex].textUnit = textureUnit;
		}

		s_data->vertices[startIndex + 0].uvCoords = texture.getUVStart();
		s_data->vertices[startIndex + 1].uvCoords = { texture.getUVStart().x, texture.getUVEnd().y };
		s_data->vertices[startIndex + 2].uvCoords = texture.getUVEnd();
		s_data->vertices[startIndex + 3].uvCoords = { texture.getUVEnd().x, texture.getUVStart().y };

		s_data->drawCount += 4;

	}

	void Renderer2DBillboard::end() {
		if (s_data->drawCount > 0) flush();
	}

	void Renderer2DBillboard::flush() {

		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->vertices.data(), sizeof(BillboardVertex) * s_data->drawCount, 0);

		glBindVertexArray(s_data->VAO->getRenderID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffers()->getRenderID());

		glDrawElements(GL_QUADS, s_data->drawCount, GL_UNSIGNED_INT, nullptr);

		s_data->drawCount = 0;
	}
}