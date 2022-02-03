#pragma once
#include "rendering/Renderer3D.h"
#include "systems/log.h"

namespace Engine
{
	class RenderComponent
	{
	public:
		RenderComponent() {}

		RenderComponent(std::shared_ptr<Material> mat)
		{
			m_mat = mat;
		}

		RenderComponent(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& mat)
		{
			m_vao = vao;
			m_mat = mat;
		}

		RenderComponent(const std::shared_ptr<ShaderRend>& shader, const std::shared_ptr<TextureRend>& texture, const glm::vec4& tint)
		{
			m_mat.reset(new Material(shader, texture, tint));
		}
		
		RenderComponent(const std::shared_ptr<ShaderRend>& shader, const glm::vec4& tint)
		{
			m_mat.reset(new Material(shader, tint));
		}
		
		RenderComponent(const std::shared_ptr<ShaderRend>& shader, const std::shared_ptr<TextureRend>& texture)
		{
			m_mat.reset(new Material(shader, texture));
		}

		std::shared_ptr<VertexArray> m_vao;
		std::shared_ptr<Material> m_mat;
	};
}