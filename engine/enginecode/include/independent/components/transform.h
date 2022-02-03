#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

	class TransformComponent {
	public:
		glm::vec3 translation = { 0.f, 0.f, 0.f };
		glm::vec3 rotation = { 0.f, 0.f, 0.f }; //Simon wants to make this a quaternion
		glm::vec3 scale = { 1.f, 1.f, 1.f };

		TransformComponent() { transform = glm::mat4(1.f); }
		TransformComponent(glm::vec3 t, glm::vec3 r, glm::vec3 s) {
			SetTransform(t, r, s);
		}

		glm::mat4& SetTransform(glm::vec3 t, glm::vec3 r, glm::vec3 s) {
			translation = t;
			rotation = r;
			scale = s;
			return UpdateTransform();
		}

		glm::mat4& SetTransform(glm::mat4 parentTransform, glm::vec3 t, glm::vec3 r, glm::vec3 s) {
			translation = t;
			rotation = r;
			scale = s;
			return UpdateTransform(parentTransform);
		}


		glm::mat4& UpdateTransform() {
			glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), rotation.x, { 1.f, 0.f, 0.f });
			glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), rotation.y, { 0.f, 1.f, 0.f });
			glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), rotation.z, { 1.f, 0.f, 1.f });
			glm::mat4 rot = rotX * rotY * rotZ;

			transform = glm::translate(glm::mat4(1.f), translation) * rot * glm::scale(glm::mat4(1.f), scale);

			return transform;
		}

		glm::mat4& UpdateTransform(glm::mat4 parentTransform) {
			transform = parentTransform * UpdateTransform();

			return transform;
		}

		glm::mat4& GetTransform() { return transform; }
	private:
		glm::mat4 transform;
	};
}