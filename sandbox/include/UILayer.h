/** \file UILayer.h*/
#pragma once


#include <rendering/Renderer2D.h>

#include <rendering/shaderDataType.h>
#include "engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



using namespace Engine;

enum class UILayerState {InActive, Activating, Active, Deactivating};
class UILayer : public Layer
{
public:
	UILayer(const char* name);
	void OnRender() override;
	void OnUpdate(float timeStep) override;
	UILayerState getState() { return m_state; }

	virtual void onKeyPressed(KeyPressedEvent& e) override;
	virtual void onMouseMoved(MouseMovedEvent& e) override;
	virtual void onMouseBtnPressed(MouseButtonPressedEvent& e) override;
	virtual void onMouseBtnReleased(MouseButtonReleasedEvent& e) override;
private:
	float m_timestep;
	std::shared_ptr<RendererCommands> enableBlendCommand;
	std::shared_ptr<RendererCommands> disableDepthCommand;
	std::shared_ptr<RendererCommands> blendFuncAlphaCommand;
	std::shared_ptr<RendererCommands> enableDepthCommand;
	std::shared_ptr<RendererCommands> disableBlendCommand;
	std::shared_ptr<RendererCommands> setStandardBlendCommand;
	
	glm::mat4 m_view2D;
	glm::mat4 m_projection2D;

	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>; //!< sceen wide uniform 
	SceneWideUniforms m_swu;

	ModalWindow m_window;
	std::vector<Quad> m_quad;
	UILayerState m_state = UILayerState::InActive;
};
