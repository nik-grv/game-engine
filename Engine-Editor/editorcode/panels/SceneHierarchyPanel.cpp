#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "include/independent/components/transform.h"
#include "include/independent/components/relationship.h"
#include "include/independent/components/render.h"
#include "include/independent/components/label.h"
#include "include/independent/components/rigidBody.h"

namespace Engine 
{

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
	}

	void SceneHierarchyPanel::SetContext()
	{

	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		auto rootView = m_registry.view<RootComponent>();
		auto root = rootView[0];
		//m_registry.each([&](auto entityID)
		//std::vector<entt::entity>& m_entities = Application::getInstance().m_entities;
		//auto view = m_registry.view<RelationshipComponent, TransformComponent>(entt::exclude<RootComponent>);
		ImGuiSomething(root);
		

		ImGui::End();
	}

	void SceneHierarchyPanel::ImGuiSomething(entt::entity entity)
	{
		//Getting the parent component
		auto relComp = m_registry.get<RelationshipComponent>(entity);
		//Getting the first child of the parent
		auto child = relComp.first;
		auto& labelCompEntt = m_registry.get<LabelComponent>(entity);
		ImGui::Text("%s", labelCompEntt.m_label);
		//Iterate through all children of the parent (if present)
		for (int i = 0; i < relComp.children; i++)
		{
			//Getting the child's label component
			auto& labelComp = m_registry.get<LabelComponent>(child);

			//Print child's label
			ImGui::Text("%s", labelComp.m_label);
			//Get the child's relationship component
			relComp = m_registry.get<RelationshipComponent>(child);

			//If the child has children
			if (relComp.children > 0)
			{
				child = relComp.first;
				//Go again
				ImGuiSomething(child);
			}

		}
	}


	//void HierarchySystem::UpdateChildren(entt::entity root)
	//{
	//	entt::registry& reg = Application::getInstance().m_registry;
	//	std::vector<entt::entity>& m_entities = Application::getInstance().m_entities;
	//	//get all objects with relationship and transform component excluding root
	//	auto view = reg.view<RelationshipComponent, TransformComponent>(entt::exclude<RootComponent>);
	//
	//	//get the root entity eg Tank Body
	//		//for loop -update transforms for all its children
	//				//for loop - for each child , update transforms for its children
	//
	//	auto& tankBodyRoot = reg.get<RelationshipComponent>(root);
	//	if (tankBodyRoot.children > 0)
	//	{
	//		for (int i = 0; i < tankBodyRoot.children; i++)
	//		{
	//			auto& parentTransform = reg.get<TransformComponent>(root);
	//			auto& nextChild = reg.get<RelationshipComponent>(tankBodyRoot.first);
	//			auto& transform = reg.get<TransformComponent>(tankBodyRoot.first);
	//			transform.UpdateTransform(parentTransform.GetTransform());
	//			if (nextChild.children)
	//			{
	//				UpdateChildren(tankBodyRoot.first);
	//			}
	//		}
	//	}
	//
	//}

}