/** EnTTLyaer.h */
#pragma once


#include <rendering/Renderer3D.h>

#include <rendering/shaderDataType.h>
#include "engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera/Camera.h"
#include <entt/entt.hpp>

namespace Engine {
	class EnTTLayer : public Layer{
	public:
		void OnUpdate(float timestep) override;
		void onKeyPressed(KeyPressedEvent& e) override;
		void OnRender() override;
		void OnResize(WindowResizeEvent& e) override;
	private:

		SceneWideUniforms m_swu;

		glm::vec3 lightData[3] = { {1.0f, 1.0f, 1.f}, {-2.f, 4.f, 6.f}, {0.f, 0.f, 0.f} };

		std::shared_ptr<Material> pyramidMaterial;

		std::shared_ptr<ShaderRend> shader;

		entt::registry& m_registry; //ESC registry whatever that is
		std::vector<entt::entity>& m_entitites; //Entities

		entt::entity m_currentCamera; //Camera thing
	};
}
