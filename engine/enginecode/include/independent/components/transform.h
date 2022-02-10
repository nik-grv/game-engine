#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine {

	class TransformComponent {
	public:
		glm::vec3 translation = { 0.f, 0.f, 0.f };
		glm::quat rotation = glm::angleAxis(0.f, glm::vec3(0.f)); //quaternion for rotation
		glm::vec3 scale = { 1.f, 1.f, 1.f };

		TransformComponent() { transform = glm::mat4(1.f); }
		TransformComponent(glm::vec3 t, glm::vec3 r, glm::vec3 s) {
			SetTransform(t, r, s);
		}

		glm::mat4& SetTransform(glm::vec3 t, glm::vec3 r, glm::vec3 s) {
			translation = t;

			glm::quat quatX = glm::angleAxis(glm::radians(r.x), glm::vec3(1.f, 0.f, 0.f));
			glm::quat quatY = glm::angleAxis(glm::radians(r.y), glm::vec3(0.f, 1.f, 0.f));
			glm::quat quatZ = glm::angleAxis(glm::radians(r.z), glm::vec3(0.f, 0.f, 1.f));

			rotation = quatX + quatY + quatZ;
			scale = s;
			return UpdateTransform();
		}

		glm::mat4& SetTransform(glm::vec3 t, glm::quat r, glm::vec3 s) {
			translation = t;
			rotation = r;
			scale = s;
		}

		glm::mat4& SetTransform(glm::mat4 parentTransform, glm::vec3 t, glm::vec3 r, glm::vec3 s) {
			translation = t;

			glm::quat quatX = glm::angleAxis(glm::radians(r.x), glm::vec3(1.f, 0.f, 0.f));
			glm::quat quatY = glm::angleAxis(glm::radians(r.y), glm::vec3(0.f, 1.f, 0.f));
			glm::quat quatZ = glm::angleAxis(glm::radians(r.z), glm::vec3(0.f, 0.f, 1.f));

			rotation = quatX + quatY + quatZ;
			scale = s;
			return UpdateTransform(parentTransform);
		}


		glm::mat4& UpdateTransform() {
			transform = glm::translate(glm::mat4(1.f), translation) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.f), scale);

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