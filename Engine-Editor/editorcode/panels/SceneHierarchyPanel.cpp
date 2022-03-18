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

		m_registry.each([&](auto entityID)
		{
			auto& tc = m_registry.get<LabelComponent>(entityID);
			ImGui::Text("%s", tc.m_label);
		});

		ImGui::End();
	}

}