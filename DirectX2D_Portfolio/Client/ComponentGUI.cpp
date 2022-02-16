#include "pch.h"
#include "ComponentGUI.h"

ComponentGUI::ComponentGUI(COMPONENT_TYPE _type) :
	m_target(nullptr),
	m_type(_type),
	m_bSubComponent(false)
{
	SetName(ComponentTypeString[(UINT)m_type]);
}

ComponentGUI::~ComponentGUI()
{
}

bool ComponentGUI::Start()
{
	if (nullptr != m_target && m_target->IsDead())
	{
		m_target = nullptr;
	}
	else if (nullptr == m_target->GetComponent(m_type))
	{
		Deactivate();
	}
	else if (nullptr != m_target->GetComponent(m_type))
	{
		Activate();
	}

	if (!IsActive())
		return false;

	ImGui::Separator();

	ImGui::BeginChild(GetName().c_str(), GetSize());

	ImGui::PushID((UINT)m_type);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.5f, 0.8f, 1.f));
	ImGui::Button(ComponentTypeString[(UINT)m_type]);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		ImGui::OpenPopup("Components##Add Component");
	if (ImGui::BeginPopup("Components##Add Component"))
	{
		SubComponent();
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor(1);
	ImGui::PopID();

	if (m_bSubComponent)
	{
		m_bSubComponent = false;
		End();
		return false;
	}

	return true;
}

void ComponentGUI::End()
{
	ImGui::EndChild();
}

void ComponentGUI::SubComponent()
{
	if (ImGui::MenuItem("Sub Component##ComponentMenu"))
	{
		m_target->SubComponent(m_type);
		Deactivate();
		m_bSubComponent = true;
	}
	
}
