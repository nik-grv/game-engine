#include "engine_pch.h"
#include "components/relationship.h"
#include "components/label.h"

namespace Engine
{


	void HierarchySystem::setChild(entt::entity parentEntity, entt::entity childEntity)
	{
		entt::registry& reg = Application::getInstance().m_registry;

		//set parent and child
		auto& parentComp = reg.get<RelationshipComponent>(parentEntity);
		auto& childComp = reg.get<RelationshipComponent>(childEntity);

		//assign a parent
		childComp.parent = parentEntity;

		//add a child to parent
		if (parentComp.children == 0)
		{
			parentComp.children = 1;
			parentComp.first = childEntity;
		}
		else
		{
			//loop through all children
			auto currentEntity = parentComp.first;
			for (int i = 1; i < parentComp.children; i++)
			{
				currentEntity = reg.get<RelationshipComponent>(currentEntity).next;
			}
			//get the last child
			auto& currentComp = reg.get<RelationshipComponent>(currentEntity);
			//add the new child as a next
			currentComp.next = childEntity;
			//update child count
			parentComp.children++;
		}

	}

	void HierarchySystem::UpdateChildren(entt::entity root)
	{
		entt::registry& reg = Application::getInstance().m_registry;
		std::vector<entt::entity>& m_entities = Application::getInstance().m_entities;
		//get all objects with relationship and transform component excluding root
		//auto view = reg.view<RelationshipComponent, TransformComponent>(entt::exclude<RootComponent>);
		
		//get the root entity eg Tank Body
			//for loop -update transforms for all its children
					//for loop - for each child , update transforms for its children
		
		auto& tankBodyRoot = reg.get<RelationshipComponent>(root);
		if (tankBodyRoot.children > 0)
		{
			for (int i = 0; i < tankBodyRoot.children; i++)
			{
				auto& parentTransform = reg.get<TransformComponent>(root);
				auto& nextChild = reg.get<RelationshipComponent>(tankBodyRoot.first);
				auto& transform = reg.get<TransformComponent>(tankBodyRoot.first);
				transform.UpdateTransform(parentTransform.GetTransform());
				  if (nextChild.children)
				  {
					  UpdateChildren(tankBodyRoot.first);
				  }
			}
		}

	}

	entt::entity HierarchySystem::GetChildEntity(entt::entity parentEntity, int childNumber)
	{
		entt::registry& reg = Application::getInstance().m_registry;

		auto parentComp = reg.get<RelationshipComponent>(parentEntity);

		if (childNumber == 0)
		{
			auto currentEntity = parentComp.first;
			return currentEntity;
		}
		else
		{
			auto currentEntity = parentComp.first;
			for (int i = 0; i < childNumber; i++)
			{
				currentEntity = reg.get<RelationshipComponent>(currentEntity).next;
			}
			return currentEntity;
		}
	}


}