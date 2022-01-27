/** \file UILayer.h*/
#pragma once


#include <rendering/Renderer2D.h>

#include <rendering/shaderDataType.h>
#include "engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



using namespace Engine;

class UILayer : public Layer
{
public:
	UILayer(const char* name);
	void OnRender() override;
private:

	std::shared_ptr<RendererCommands> enableBlendCommand;
	std::shared_ptr<RendererCommands> disableDepthCommand;
	std::shared_ptr<RendererCommands> blendFuncAlphaCommand;
	std::shared_ptr<RendererCommands> enableDepthCommand;
	std::shared_ptr<RendererCommands> disableBlendCommand;
	glm::mat4 m_view2D;
	glm::mat4 m_projection2D;
	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>; //!< sceen wide uniform 
	SceneWideUniforms m_swu;
	std::vector<Quad> m_quad;
};
