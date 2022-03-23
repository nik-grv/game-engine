/** \file Renderer2D.h*/
#pragma once

#include "rendering/RendererShared.h"
#include "ft2build.h"
#include "freetype/freetype.h"
#include "UI/modalWindow.h"

#include <array>
#include <vector>

namespace Engine {

	/**
	\class VertexRenderer2D
	* \brief class to batch 2d rendering objects and text
	*/
	class VertexRenderer2D
	{
	public:
		VertexRenderer2D() = default; //!< default constructor

		//! Overloaded constructor to initialise a vertex renderer 2D
		/*!
		\param position glm::vec4 - position of vertex
		\param uvCoordinares glm::vec2 - UV coordinates of vertex
		\param textureUnit uint32_t - texture unit of vertex
		\param tint glm::vec4 - tint of vertex
		*/
		VertexRenderer2D(const glm::vec4& pos, const glm::vec2& uvCoords, uint32_t textUnit, const glm::vec4& tint) :
			m_position(pos), m_uvCoords(uvCoords), m_textUnits(textUnit),m_tint(pack(tint)) {}
		glm::vec4 m_position; //!< position of the quad
		glm::vec2 m_uvCoords; //!< uv coords of the quad
		uint32_t m_textUnits; //!< texture unit
		uint32_t m_tint; //!< tint for the quad
		static VertexBufferLayout m_layout; //!< vertex buffer layout
		static uint32_t pack(const glm::vec4& tint); //!< pack tint 
	};

	/**
	\class Quad
	* \brief class to render 2D quad
	*/
	class Quad
	{
	public:
		//! Function to create a quad with centre and half extent values
		/*!
		\param centre glm::vec2 - centre of quad
		\param halfExtents glm::vec2 - half extneds of quad
		*/
		static Quad createCentreHalfExtens(const glm::vec2& centre, const glm::vec2& halfExtents);
	
	private:
		glm::vec3 m_translate = glm::vec3(0.0f); //!< translation vector
		glm::vec3 m_scale = glm::vec3(1.0f); //!< scale vector
		friend class Renderer2D; //!< settings Renderer2D as a friend class to be used by the Quad
	};
	/**
	\class Renderer2D
	* \brief class to render 2D geometry (non batched)
	*/
	class Renderer2D
	{
	public:
		static void init(); //!< initialise the renderer
		
		//! Function to begin rendering a 2D scene
		/*!
		\param sceneWideUniforms SceneWideUniforms - scene wide uniform values 
		*/
		static void begin(const SceneWideUniforms& sceneWideUniforms);

		//! Function to submit a geometry for rendering a Tinted 2D quad
		/*!
		\param quad Quad - 2D quad
		\param tint glm::vec4 - tint of the quad
		\param angle float - angle of the quad
		\param inDegree bool - is angle in degrees
		*/
		static void submit(const Quad& quad, const glm::vec4& tint, float angle = 0.0f, bool inDeg = false);  
		
		//! Function to submit a geometry for rendering a Textured 2D quad
		/*!
		\param quad Quad - 2D quad
		\param texture SubTexture - texture of the quad
		\param angle float - angle of the quad
		\param inDegree bool - is angle in degrees
		*/
		static void submit(const Quad& quad, const SubTexture& texture, float angle = 0.0f, bool inDeg = false); 

		//! Function to submit a geometry for rendering a Tinted and Textured 2D quad
		/*!
		\param quad Quad - 2D quad
		\param tint glm::vec4 - tint of the quad
		\param texture SubTexture - texture of the quad
		\param angle float - angle of the quad
		\param inDegree bool - is angle in degrees
		*/
		static void submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture, float angle = 0.0f, bool inDeg = false);
		
		//! Function to submit a geometry for a Tinted text character
		/*!
		\param character char - a character 
		\param position glm::vec2 - position of the character
		\param advance float - offset in position
		\param tint glm::vec4 - tint of the character
		*/
		static void submit(char character, const glm::vec2& pos, float& advance, const glm::vec4& tint);

		//! Function to submit a geometry for a Tinted text string
		/*!
		\param string string - string of characters
		\param position glm::vec2 - position of the character
		\param advance float - offset in position
		\param tint glm::vec4 - tint of the character
		*/
		static void submit(const char * stringText, const glm::vec2& pos, const glm::vec4& tint);

		static void end(); //!< end rendering current scene
		static void flush(); //!< render all the geometry
		static glm::ivec2 getTextSize(const char* text);

	private:
		

		/** \struct GlyphData
		*\brief properties for the character glyph
		\param size glm::vec2 - sizde of the character
		\param bearing glm::vec2 - bearing of the character glyph
		\param advance float - offset value for the character
		\param texture SubTexture - texture for the glyph
		\param character char - the text character
		*/
		struct GlyphData
		{
			glm::vec2 size; //!< size of the character glyph 
			glm::vec2 bearing;//!< bearing of the character glyph
			float advance; //!< advance for the character glyph
			SubTexture subTexture;//!< subtexture for the char glyph
			unsigned char ch; //!< character
		};


		/** \struct InternalData
		*\brief all Renderer properties used for rendering to be used as a static object
		\param defaultTexture TextureRend - default white texture
		\param texture SubTexture - default subtexture
		\param tint glm::vec4 - default white tint (albedo)
		\param shader ShaderRend - shader used for the 2D object
		\param VAO VertexArray - vertex array for the 2D quad
		\param UBO UniformBuffer - uniform buffer for the 2D quad
		\param model glm::mat4 - model matrix
		\param textureUnits int32_t - texture units
		\param quadPos glm::vec4 - position of quad
		\param batchSize uint32_t - batch size per draw call
		\param drawCount uint32_t - draw count
		\param vertices VertexRenderer2D - quad vertices
		
		\param fontLibrary FT_Library - freetype library
		\param fontFace FT_Face - freetype font face
		\param firstChar char - first typeable char ascii code (32 = Space)
		\param lastChar char - last typeable char ascii code (126 = ~)
		\param glyphAtlas TextureAtlas - texture atlas for the font 
		\param glyphData GlyphData - struct of glyph data 
		*/
		struct InternalData
		{
			std::shared_ptr<TextureRend> defTexture; //!< empty white texture
			SubTexture defaultSubTexture; //!< default sub texture
			glm::vec4 defTint; //!< def white tint color (albedo)
			std::shared_ptr<ShaderRend> shader; //!< shader
			std::shared_ptr<VertexArray> quadVAO; //!< quad vertex array
			std::shared_ptr<UniformBuffer> quadUBO; //!< quad uniform buffer
			glm::mat4 model; //!< model matrix
			std::array<int32_t, 32> textUnits; //!< array of texture units
			std::array<glm::vec4,4> quadPos; //!< quad vertices
			static const uint32_t batchSize = 8192; //!< batch size per draw call
			uint32_t drawCount; //!< draw count 
			std::vector<VertexRenderer2D> vertices; //!< quad vertices
				
			FT_Library ftLib; //!< freetype library
			FT_Face ftFace; //!< freetype font face
			unsigned char FIRST_CHAR = 32; //!<first typeable char ascii code (32 = Space)
			unsigned char LAST_CHAR = 126; //!< last typeable char ascii code (126 = ~)
			TextureAtlas glyphFontAtlas; //!< texture atlas for font
			std::vector<GlyphData> glyphData; //!< vector of glyph data
		};


		static std::shared_ptr<InternalData> s_data; //!< internal data of the renderer

		

		//! Function to convert the buffer to RGBA format
		/*!
		\param srcBuffer char - source buffer
		\param destBuffer char - destination buffer
		\param width uint32_t - width of buffer
		\param height uint32_t - height of buffer
		*/
		static void RToRGBA(unsigned char* srcBuffer, unsigned char* destBuffer, uint32_t width, uint32_t height);

	};
}