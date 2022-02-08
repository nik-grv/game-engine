/** \file Renderer3D.h*/
#pragma once

#include "rendering/RendererShared.h"

namespace Engine {

	class Renderer3DVertex
	{
	public:
		Renderer3DVertex() = default;
		Renderer3DVertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv) :
			position(pos), normal(norm), uvCoords(uv) {}
		 glm::vec3 position;
		 glm::vec3 normal; 
		 glm::vec2 uvCoords;
		 static VertexBufferLayout layout;
	};


	/**
	\class Material
	* \brief class to hold shader and uniform data associated with the shader
	*/
	class Material
	{
	public:


		//! Overloaded constructor to set material with shader
		/*!
		\param shader ShaderRend - shader 
		*/
		Material(const std::shared_ptr<ShaderRend>& shader) :
			m_shader(shader), m_flags(0), m_texture(nullptr), m_tint(glm::vec4(0)) {}

		//! Overloaded constructor to set material with shader texture and tint
		/*!
		\param shader ShaderRend - shader
		\param texture TextureRend - texture to be used for material
		\param tint glm::vec4 - tint of material
		*/
		Material(const std::shared_ptr<ShaderRend>& shader, const std::shared_ptr<TextureRend>& texture , const glm::vec4 tint) :
			m_shader(shader), m_texture(texture), m_tint(tint) 
		{
			SetFlag(flag_Texture | flag_tint);
		}

		//! Overloaded constructor to set material with shader and texture
		/*!
		\param shader ShaderRend - shader
		\param texture TextureRend - texture to be used for material
		*/
		Material(const std::shared_ptr<ShaderRend>& shader, const std::shared_ptr<TextureRend>& texture) :
			m_shader(shader), m_texture(texture), m_tint(glm::vec4(1.0f)) 
		{
			SetFlag(flag_Texture);
		}

		//! Overloaded constructor to set material with shader and tint
		/*!
		\param shader ShaderRend - shader
		\param tint glm::vec4 - tint of material
		*/
		Material(const std::shared_ptr<ShaderRend>& shader, const glm::vec4 tint) :
			m_shader(shader), m_texture(nullptr), m_tint(tint)
		{
			SetFlag(flag_tint);
		}

		inline std::shared_ptr<ShaderRend> getShader() const { return m_shader; } //!< get shader
		inline std::shared_ptr<TextureRend> getTexture() const { return m_texture; } //!< get texture
		inline glm::vec4 getTint() const { return m_tint; } //!< get texture

		//! check if flag is set
		/*!
		\param flag uint32_t - flag for material
		*/
		bool isFlagSet(uint32_t flag) const { return m_flags & flag; }


		//! set texture for the material
		/*!
		\param texture TextureRend - texture to be used for material
		*/
		void setTexture(const std::shared_ptr<TextureRend>& texture) { m_texture = texture; }

		//! Set tint for the material
		/*!
		\param tint glm::vec4 - tint of vertex
		*/
		void setTint(const glm::vec4 tint) { m_tint= tint; } //!< set tint

		constexpr static uint32_t flag_Texture = 1 << 0; //!< 01
		constexpr static uint32_t flag_tint = 1 << 1; //!< 10

	private:
		uint32_t m_flags = 0; //!< bitwise representation of shader settings
		std::shared_ptr<ShaderRend> m_shader; //!< shader for the material;
		std::shared_ptr<TextureRend> m_texture; //!< shader for the material;
		glm::vec4 m_tint; //!< color for the material;

		//! set flag for the material
		/*!
		\param flag uint32_t - flag for material
		*/
		void SetFlag(uint32_t flag) { m_flags = m_flags | flag; }
	};
	
	
	/**
	\class Renderer3D
	* \brief class to render 3D geometry (non batched)
	*/
	class Renderer3D
	{
	public:
		static void init(); //!< initialise the renderer

		//! Function to begin rendering a 3D scene
		/*!
		\param sceneWideUniforms SceneWideUniforms - scene wide uniform values
		*/
		static void begin(const SceneWideUniforms& sceneWideUniforms);

		//! Function to submit geometry for rendering
		/*!
		\param geometry VertexArray - vertex arrray for the 3D object
		\param mat Material - material of the 3D object
		\param model glm::mat4 - matrix model 
		*/
		static void submit(std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material>& mat, const glm::mat4& model); 
		static void end(); //!< end rendering current scene

		//! Attach shader to the UBOs
		/*!
		\param shader ShaderRend - shader
		*/
		static void registerShader(std::shared_ptr<ShaderRend>& shader);

	private:

		/** \struct InternalData
		*\brief all Renderer properties used for rendering to be used as a static object
		\param sceneWideUniforms SceneWideUniforms - scene wide uniform values
		\param defaultTexture TextureRend - default white texture
		\param shader ShaderRend - default subtexture
		\param cameraUBO UniformBuffer - camera UBO data
		\param lightsUBO UniformBuffer - lights UBO data
		\param tint glm::vec4 - default white tint (albedo)
		\param textureUnitManager TextureUnitManager - texture unit workshop
		\param textureUnit uint32_t - texture unit
		\param UBO UniformBuffer - uniform buffer for the 2D quad
		\param model glm::mat4 - model matrix
		\param textureUnits int32_t - texture units
		\param lightPos glm::vec3 - position of light
		\param viewPos glm::vec3 - view position
		\param lightCol glm::vec3 - light color
		*/
		struct InternalData
		{
			SceneWideUniforms sceneWideUniforms; //!< to be replaced with UBO in future
			std::shared_ptr<TextureRend> defTexture; //!< empty white texture;
			std::shared_ptr<ShaderRend> shader; //!< shader for the 3d models
			std::shared_ptr<UniformBuffer> cameraUBO; //!< camera ubo data
			std::shared_ptr<UniformBuffer> lightUBO; //!< lights ubo data
			std::shared_ptr<VertexArray> VAO; //!< All static meshes
			glm::vec4 defTint; //!< def white tint color (albedo)
			TextureUnitManager m_TextureUnitManager; //!< texture unit manager
			uint32_t m_TextureUnit;	//!<texture unit
			glm::vec3 lightPos = glm::vec3(1.f, 4.f, 6.f); //!< light pos
			glm::vec3 viewPos= glm::vec3(0.f, 0.f, 0.f); //!< view pos
			glm::vec3 lightCol = glm::vec3(1.f, 1.f, 1.f); //!< light col

			std::vector<glm::mat4> modelInstanceData;
			std::vector<uint32_t> tintInstanceData;
			std::vector<uint32_t> texInstanceData;

		};

		static std::shared_ptr<InternalData> s_data; //!< internal data of the renderer
	};
}