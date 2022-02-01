/** \file Renderer3D.cpp*/ 
#include <engine_pch.h>
#include "rendering/Renderer3D.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

namespace Engine {

	std::shared_ptr<Renderer3D::InternalData> Renderer3D::s_data = nullptr;

	void Renderer3D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePixel[4] = { 255,255,255,255 };
		s_data->defTexture.reset(TextureRend::create(1, 1, 4, whitePixel));
		s_data->defTint = { 1.f,1.f,1.f,1.f};

		UniformBufferLayout cameraLayout = { {"u_projection",ShaderDataType::Mat4} , {"u_view",ShaderDataType::Mat4} };
		s_data->cameraUBO.reset(UniformBuffer::create(cameraLayout));

		UniformBufferLayout lightLayout = { {"u_lightPos",ShaderDataType::Float3},{"u_viewPos",ShaderDataType::Float3}, {"u_lightColour",ShaderDataType::Float3} };
		s_data->lightUBO.reset(UniformBuffer::create(lightLayout));

		s_data->lightUBO->uploadDataToUB("u_lightPos", glm::value_ptr(s_data->lightPos));
		s_data->lightUBO->uploadDataToUB("u_viewPos", glm::value_ptr(s_data->viewPos));
		s_data->lightUBO->uploadDataToUB("u_lightColour", glm::value_ptr(s_data->lightCol));
		
	}

	void Renderer3D::begin(const SceneWideUniforms& sceneWideUniforms)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, s_data->cameraUBO->getRenderID());
		s_data->sceneWideUniforms = sceneWideUniforms;
		s_data->cameraUBO->uploadDataToUB("u_projection", sceneWideUniforms.at("u_projection").second);
		s_data->cameraUBO->uploadDataToUB("u_view", sceneWideUniforms.at("u_view").second);

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->lightUBO->getRenderID());
		s_data->lightUBO->uploadDataToUB("u_viewPos", glm::value_ptr(s_data->viewPos));
	}

	void Renderer3D::submit(std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material>& mat, const glm::mat4& model)
	{
		//bind the shader
		glUseProgram(mat->getShader()->getRenderID());

		//apply scenwideuniforms 
		for (auto& dataPair : s_data->sceneWideUniforms)
		{
			const char* uniformName = dataPair.first;
			ShaderDataType& sdt = dataPair.second.first;
			void* address = dataPair.second.second;

			switch (sdt)
			{
			case ShaderDataType::Int:
				mat->getShader()->uploadInt(uniformName, *(int*)address);
				break;
			case ShaderDataType::Float3:
				mat->getShader()->uploadFloat3(uniformName, *(glm::vec3*)address);
				break;
			case ShaderDataType::Float4:
				mat->getShader()->uploadFloat4(uniformName, *(glm::vec4*)address);
				break;
			case ShaderDataType::Mat4:
				mat->getShader()->uploadMat4(uniformName, *(glm::mat4*)address);
				break;
			}
		}
		//apply material uniforms
		mat->getShader()->uploadMat4("u_model", model);

		std::shared_ptr<TextureRend> texture;
		if (mat->isFlagSet(Material::flag_Texture))
			texture = mat->getTexture();
		else
			texture = s_data->defTexture;
		
		uint32_t texUnit;
		if (RendererShared::s_tum.getUnit(texture->getRenderID(), texUnit))
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture->getRenderID());
		}
			
		mat->getShader()->uploadInt("u_texData", texUnit);

		if (mat->isFlagSet(Material::flag_tint))
		{
			mat->getShader()->uploadFloat4("u_tint", mat->getTint());
		}
		else
		{
			mat->getShader()->uploadFloat4("u_tint", s_data->defTint);
		}

		//bind geometry (VBO and IBO)
		glBindVertexArray(geometry->getRenderID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->getIndexBuffers()->getRenderID());

		//submit draw call
		glDrawElements(GL_TRIANGLES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer3D::end()
	{
		s_data->sceneWideUniforms.clear();
	}
	void Renderer3D::registerShader(std::shared_ptr<ShaderRend>& shader)
	{
		s_data->cameraUBO->attachShaderBlock(shader, "ubo_camera");
		s_data->lightUBO->attachShaderBlock(shader, "ubo_lights");
	}
}