/** \file Renderer2D.cpp*/
#include <engine_pch.h>
#include <glad/glad.h>
#include "rendering/Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <systems/log.h>
#include <numeric>

#include "ft2build.h"
#include "freetype/freetype.h"

namespace Engine 
{
	std::shared_ptr<Renderer2D::InternalData> Renderer2D::s_data = nullptr;
	VertexBufferLayout VertexRenderer2D::m_layout = VertexBufferLayout({ ShaderDataType::Float4,ShaderDataType::Float2, ShaderDataType::FlatInt, {ShaderDataType::Byte4,true} });
	TextureUnitManager RendererShared::s_tum = TextureUnitManager(32);

	void Renderer2D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePixel[4] = { 255,255,255,255 };
		s_data->defTexture.reset(TextureRend::create(1, 1, 4, whitePixel));

		s_data->defaultSubTexture = SubTexture(s_data->defTexture, glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f));

		s_data->defTint = { 1.f,1.f,1.f,1.f };
		s_data->model = glm::mat4(1.0f);
		s_data->shader.reset(ShaderRend::create("./assets/shaders/quadColour.glsl"));

		for (int i = 0; i < s_data->textUnits.size(); i++)
		{
			s_data->textUnits[i] = i;
		}

		UniformBufferLayout layout = { {"u_view",ShaderDataType::Mat4},{"u_projection",ShaderDataType::Mat4} };
		s_data->quadUBO.reset(UniformBuffer::create(layout));


		s_data->quadPos[0] = glm::vec4(-0.5, -0.5, 1.0f, 1.0f);
		s_data->quadPos[1] = glm::vec4(-0.5, 0.5, 1.0f, 1.0f);
		s_data->quadPos[2] = glm::vec4(0.5, 0.5f, 1.0f, 1.0f);
		s_data->quadPos[3] = glm::vec4(0.5f, -0.5f, 1.0f, 1.0f);

		s_data->vertices.resize(s_data->batchSize);

		s_data->vertices[0] = VertexRenderer2D(s_data->quadPos[0], glm::vec2(0.0f, 0.0f), 0, glm::vec4(1.0f));
		s_data->vertices[1] = VertexRenderer2D(s_data->quadPos[1], glm::vec2(0.0f, 1.0f), 0, glm::vec4(1.0f));
		s_data->vertices[2] = VertexRenderer2D(s_data->quadPos[2], glm::vec2(1.0f, 1.0f), 0, glm::vec4(1.0f));
		s_data->vertices[3] = VertexRenderer2D(s_data->quadPos[3], glm::vec2(1.0f, 0.0f), 0, glm::vec4(1.0f));

		std::vector<uint32_t> indices(s_data->batchSize);
		//fill the vector from begin to end with the successive values of 'value' -> 0 (in this case)
		std::iota(indices.begin(), indices.end(), 0);

		std::shared_ptr<VertexBuffer> quadVbo;
		std::shared_ptr<IndexBuffer> quadIbo;

		s_data->quadVAO.reset(VertexArray::create());
		quadVbo.reset(VertexBuffer::create(s_data->vertices.data(), sizeof(VertexRenderer2D) * s_data->vertices.size(), VertexRenderer2D::m_layout));
		quadIbo.reset(IndexBuffer::create(indices.data(), indices.size()));
		s_data->quadVAO->addVertexBuffer(quadVbo);
		s_data->quadVAO->setIndexBuffer(quadIbo);

		s_data->quadUBO->attachShaderBlock(s_data->shader, "ubo_camera");

		//FONT INIT------
		//Initialise free type lib
		const char* fontPath = "./assets/fonts/arial.ttf";
		int32_t charSize = 25;

		if (FT_Init_FreeType(&s_data->ftLib))
		{
			Log::error("ERROR : Freetype could not initialsied");
		}
		//set font face
		if (FT_New_Face(s_data->ftLib, fontPath, 0, &s_data->ftFace))
		{
			Log::error("ERROR : Freetype could not load font at : {0}", fontPath);

		}
		//set font size
		if (FT_Set_Pixel_Sizes(s_data->ftFace, 0, charSize))
		{
			Log::error("ERROR : Freetype font size could not be set {0} ", charSize);
		}

		//fill texture atlas
		s_data->glyphData.resize(s_data->LAST_CHAR - s_data->FIRST_CHAR + 1);

		std::vector<std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>> tmpGlyphData; //!< vector of glyph data
		tmpGlyphData.resize(s_data->LAST_CHAR - s_data->FIRST_CHAR + 1);
		
		for (unsigned char character = s_data->FIRST_CHAR; character <= s_data->LAST_CHAR; character++)
		{
			if (FT_Load_Char(s_data->ftFace, character, FT_LOAD_RENDER))
			{
				Log::error("ERROR : Could not load glyph for char {0}", character);
			}
			else
			{
				auto& pair1 = tmpGlyphData.at(character - s_data->FIRST_CHAR);
				pair1.first = character;
				GlyphData& glyphData = pair1.second.first;

				//set glyph data
				glyphData.ch = character;
				glyphData.size = glm::vec2(s_data->ftFace->glyph->bitmap.width, s_data->ftFace->glyph->bitmap.rows);
				glyphData.bearing = glm::vec2(s_data->ftFace->glyph->bitmap_left, -s_data->ftFace->glyph->bitmap_top);
				//calc advance
				glyphData.advance = static_cast<float>(s_data->ftFace->glyph->advance.x >> 6);
				
				//set glyph buffer dimensions
				glm::vec2 glyphBufferDimensions = glyphData.size;
				uint32_t glyphBufferSize = glyphBufferDimensions.x * glyphBufferDimensions.y * 4 * sizeof(unsigned char);

				//unsigned char* glyphBuffer = static_cast<unsigned char*>(malloc(glyphBufferSize));
				pair1.second.second = static_cast<unsigned char*>(malloc(glyphBufferSize));

				RToRGBA(s_data->ftFace->glyph->bitmap.buffer, pair1.second.second, glyphData.size.x, glyphData.size.y);
			}

		}

		std::sort(tmpGlyphData.begin(), tmpGlyphData.end(),
			[](std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>& a, std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>& b)
		{
			return a.second.first.size.x > b.second.first.size.x;
		});

		for (unsigned char CURR_GLYPH = 0; CURR_GLYPH <= s_data->LAST_CHAR - s_data->FIRST_CHAR; CURR_GLYPH++)
		{
			auto& GD = tmpGlyphData.at(CURR_GLYPH).second.first;
			auto glyphBuffer = tmpGlyphData.at(CURR_GLYPH).second.second;
			unsigned char ch = tmpGlyphData.at(CURR_GLYPH).first;
			GD.subTexture = SubTexture(s_data->glyphFontAtlas.getBaseTexture(), glm::vec2(0.0f), glm::vec2(1.0f));

			if (!s_data->glyphFontAtlas.add(GD.size.x, GD.size.y, 4, glyphBuffer, GD.subTexture))
			{
				Log::error("No Space for Glyph {0}", ch);
			}
			s_data->glyphData.at(CURR_GLYPH) = GD;
			free(glyphBuffer);
		}

		std::sort(s_data->glyphData.begin(), s_data->glyphData.end(),
			[](GlyphData& a, GlyphData& b)
		{
			return a.ch < b.ch;
		});

	}

	void Renderer2D::begin(const SceneWideUniforms& sceneWideUniforms)
	{
		
		//reset the draw count
		s_data->drawCount = 0;
		//bind shader
		glUseProgram(s_data->shader->getRenderID());

		//upload scenewide uniforms
		s_data->shader->uploadIntArray("u_texData", s_data->textUnits.data(), 32);
		glBindBuffer(GL_UNIFORM_BUFFER, s_data->quadUBO->getRenderID());
		s_data->quadUBO->uploadDataToUB("u_view",sceneWideUniforms.at("u_view").second);
		s_data->quadUBO->uploadDataToUB("u_projection",sceneWideUniforms.at("u_projection").second);

	}

	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, float angle, bool inDeg)
	{
		Renderer2D::submit(quad, tint, s_data->defaultSubTexture,angle,inDeg);
	}

	void Renderer2D::submit(const Quad& quad, const SubTexture& texture, float angle, bool inDeg)
	{
		Renderer2D::submit(quad, s_data->defTint, texture,angle,inDeg);
	}

	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture, float angle, bool inDeg)
	{
		if (inDeg)
			angle = glm::radians(angle);

		if (s_data->drawCount + 4 > s_data->batchSize) 
			flush();

		uint32_t texUnit;
		if (RendererShared::s_tum.full())
			flush();

		if (RendererShared::s_tum.getUnit(texture.getBaseTexture()->getRenderID(), texUnit))
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture.getBaseTexture()->getRenderID());
		}
		if(inDeg)
			s_data->model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), quad.m_translate),angle,glm::vec3(0.0f,0.0f,1.0f)), quad.m_scale);
		else
			s_data->model = glm::scale(glm::translate(glm::mat4(1.0f), quad.m_translate), quad.m_scale);

		uint32_t pTint = VertexRenderer2D::pack(tint);

		uint32_t startIndex = s_data->drawCount;
		for (int i = 0; i < 4; i++)
		{
			s_data->vertices[i + startIndex].m_position = s_data->model * s_data->quadPos[i];
			s_data->vertices[i + startIndex].m_tint = pTint;
			s_data->vertices[i + startIndex].m_textUnits = texUnit;
		}

		s_data->vertices[startIndex + 0].m_uvCoords = texture.getUVStart();
		s_data->vertices[startIndex + 1].m_uvCoords = glm::vec2(texture.getUVStart().x, texture.getUVEnd().y);
		s_data->vertices[startIndex + 2].m_uvCoords = texture.getUVEnd();
		s_data->vertices[startIndex + 3].m_uvCoords = glm::vec2(texture.getUVEnd().x, texture.getUVStart().y);

		s_data->drawCount += 4;
	}

	void Renderer2D::submit(char character, const glm::vec2& pos, float& advance, const glm::vec4& tint)
	{

		if (FT_Load_Char(s_data->ftFace, character, FT_LOAD_RENDER))
		{
			Log::error("ERROR : Could not load glyph for char {0}", character);
		}

		//get glyph data
		GlyphData& GD = s_data->glyphData.at(character - s_data->FIRST_CHAR);

		//calc quad for glyph data
		glm::vec2 glyphHalfExtents(GD.size* 0.5f);
		glm::vec2 glyphCentre = (pos + GD.bearing) + glyphHalfExtents;

		Quad quad = Quad::createCentreHalfExtens(glyphCentre, glyphHalfExtents);

		advance = GD.advance;
		//submit quad
		submit(quad, tint, GD.subTexture);	
	}

	void Renderer2D::submit(const char* stringText, const glm::vec2& pos, const glm::vec4& tint)
	{
		uint32_t length = strlen(stringText);
		float advance = 0.0f;
		float posX = pos.x;
		for (int32_t i = 0; i < length; i++)
		{
			submit(stringText[i], glm::vec2(posX,pos.y), advance, tint);
			posX += advance;
		}
	}

	void Renderer2D::end()
	{
		if (s_data->drawCount > 0)
			flush();
	}

	void Renderer2D::flush()
	{
		//once per batch...
		s_data->quadVAO->getVertexBuffers().at(0)->edit(s_data->vertices.data(), sizeof(VertexRenderer2D) * s_data->drawCount, 0);

		glBindVertexArray(s_data->quadVAO->getRenderID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->quadVAO->getIndexBuffers()->getRenderID());

		glDrawElements(GL_QUADS, s_data->drawCount, GL_UNSIGNED_INT, nullptr);

		//reset the draw count 
		s_data->drawCount = 0;
	}

	glm::ivec2 Renderer2D::getTextSize(const char* text)
	{
		glm::ivec2 result(0);
		uint32_t len = strlen(text);
		float advance = 0.f;
		
		for (int32_t i = 0; i < len; i++)
		{
			GlyphData& gd = s_data->glyphData.at(text[i] - s_data->FIRST_CHAR);
			result.x += gd.advance;
			result.y = std::max(result.y, static_cast<int>(gd.size.y));
		}
		return result;
	}
	void Renderer2D::RToRGBA(unsigned char* srcBuffer, unsigned char* destBuffer, uint32_t width, uint32_t height)
	{
		unsigned char* pointerWalker = destBuffer;
		for (int32_t i = 0; i < height; i++)
		{
			for (int32_t j = 0; j < width; j++)
			{
				*pointerWalker = 255; pointerWalker++;
				*pointerWalker = 255; pointerWalker++;
				*pointerWalker = 255; pointerWalker++;
				*pointerWalker = *srcBuffer; //set alpha channel
				pointerWalker++;	//Go to R of next pixel
				srcBuffer++;			//Go to mohochrome pixel
			}
		}
	}

	Quad Quad::createCentreHalfExtens(const glm::vec2& centre, const glm::vec2& halfExtents)
	{
		Quad result;

		result.m_translate = glm::vec3(centre, 0.0f);
		result.m_scale = glm::vec3(halfExtents * 2.0f, 1.0f);

		return result;
	}

	uint32_t VertexRenderer2D::pack(const glm::vec4& tint)
	{
		uint32_t result = 0;

		uint32_t r = (static_cast<uint32_t>(tint.r * 225.f)) << 0;
		uint32_t g = (static_cast<uint32_t>(tint.g * 225.f)) << 8;
		uint32_t b = (static_cast<uint32_t>(tint.b * 225.f)) << 16;
		uint32_t a = (static_cast<uint32_t>(tint.a * 225.f)) << 24;

		result = (r | g | b | a);
		return result;
	}

}