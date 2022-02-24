#pragma once

#include "engine.h"
#include <assimpLoader.h>

using namespace Engine;

class TankController : public Engine::NativeScript {
public:
	TankController(entt::entity& entity, float movementSpeed, bool active) :
		NativeScript(entity),
		m_movementSpeed(movementSpeed),
		m_active(active)
	{
	
		Loader::ASSIMPLoad("./assets/models/Tank/shell.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);

		m_shellVAO.reset(VertexArray::create());
		VertexBufferLayout cubeBufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
		m_shellVBO.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));
		m_shellIBO.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_shellVAO->addVertexBuffer(m_shellVBO);
		m_shellVAO->setIndexBuffer(m_shellIBO);

		entt::registry& registry = Application::getInstance().m_registry;
		std::shared_ptr<ShaderRend> shader = registry.get<RenderComponent>(entity).m_mat->getShader();

		shellMat.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(.5f)));
	
	};

	virtual void OnUpdate(float time)
	{
		float desRotVel = 0.f;
		rp3d::Vector3 desSpeed = rp3d::Vector3(0.f, 0.f, 0.f);

		entt::registry& registry = Application::getInstance().m_registry;
		auto& rb = registry.get<RigidBodyComponent>(m_entity);
		auto& tc = registry.get<TransformComponent>(m_entity);
		auto& t = tc.GetTransform();
		rp3d::Vector3 forward(t[2][0], t[2][1], t[2][2]);
		fwd = -forward;
		if (InputPoller::isKeyPressed(NG_KEY_UP))
		{
			desSpeed = fwd * m_movementSpeed;
		}

		if (InputPoller::isKeyPressed(NG_KEY_DOWN))
		{
			desSpeed = fwd * -m_movementSpeed;
		}

		if (InputPoller::isKeyPressed(NG_KEY_LEFT))
		{
			desRotVel = -3.f;
		}

		if (InputPoller::isKeyPressed(NG_KEY_RIGHT))
		{
			desRotVel = 3.f;
		}

		float mass = rb.m_body->getMass();

		float deltaRotV = desRotVel - rb.m_body->getAngularVelocity().y;
		float rotForceY = mass * (deltaRotV / time);

		rp3d::Vector3 deltaV = desSpeed - rb.m_body->getLinearVelocity();
		rp3d::Vector3 moveForce =  (deltaV / time) * mass;

		rb.m_body->applyTorque(rp3d::Vector3(0.f, rotForceY, 0.f));
		rb.m_body->applyForceToCenterOfMass(moveForce);
	}


	virtual void OnMouseBtnPressed(MouseButtonPressedEvent& e) 
	{
		//projectle shooting....

		//shoot projectile with mouse click
		if (e.getButton() == 0)
		{

			std::vector<entt::entity>& m_entities = Application::getInstance().m_entities;
			entt::registry& registry = Application::getInstance().m_registry;
			bool found = false;
			int i;
			for (i = 0; i < m_entities.size(); i++)
			{
				if (m_entities[i] == entt::null)
					continue;
			}

			entt::entity projectileEntity = registry.create();
			if (i == m_entities.size())
				m_entities.push_back(projectileEntity);
			else
				m_entities[i] = projectileEntity;

			registry.emplace<LabelComponent>(projectileEntity, "Projectile");

			//auto camTransform = glm::inverse(m_camera.getCameraViewMatrix());
			auto& fireTransform = registry.get<TransformComponent>(m_entities[5]).GetTransform();
			glm::vec3 forward = { fwd.x,fwd.y ,fwd.z };
			glm::vec3 camPos = { fireTransform[3][0],fireTransform[3][1] ,fireTransform[3][2] };
			auto projTransform = registry.emplace<TransformComponent>(projectileEntity, camPos - forward * 1.0f, glm::vec3(0.0f), glm::vec3(0.25f));

			registry.emplace<RenderComponent>(projectileEntity, m_shellVAO, shellMat);
			registry.emplace<RelationshipComponent>(projectileEntity);
			//HierarchySystem::setChild(m_entities[0], projectileEntity);

			auto projectiel_rb = registry.emplace<RigidBodyComponent>(projectileEntity, RigidBodyType::Dynamic, projTransform.GetTransform());
			registry.emplace<SphereColliderComponent>(projectileEntity, projectiel_rb, 0.25f);
			glm::vec3 force = (-forward * 500.5f);
			projectiel_rb.m_body->applyForceToCenterOfMass(rp3d::Vector3(force.x, force.y + 20.f, force.z - 1500.f));
		}
	}
	
	virtual void OnMouseMoved(MouseMovedEvent& e) 
	{

	}

protected:
	float m_movementSpeed;
	float m_timeElapsed;
	bool m_active = true;
	std::shared_ptr<VertexArray> m_shellVAO;
	std::shared_ptr<VertexBuffer> m_shellVBO;
	std::shared_ptr<IndexBuffer> m_shellIBO;
	std::shared_ptr<Material> shellMat;
	rp3d::Vector3 fwd;

};