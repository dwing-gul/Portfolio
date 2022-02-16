#include "pch.h"
#include "CComponentGUI.h"

CComponentGUI::CComponentGUI(COMPONENT_TYPE _type) :
	m_target(nullptr),
	m_componentType(_type),
	m_bSubComponent(false)
{
	if (m_componentType == COMPONENT_TYPE::SCRIPT)
	{
		SetName("Script");
	}
	else
		SetName(ComponentTypeString[(UINT)m_componentType]);
}

CComponentGUI::~CComponentGUI()
{
}

bool CComponentGUI::Start()
{
	if (nullptr != m_target && m_target->IsDead())
	{
		m_target = nullptr;
	}
	else if (nullptr == m_target->GetComponent(m_componentType))
		Deactivate();
	else if (nullptr != m_target->GetComponent(m_componentType))
		Activate();

	if (!IsActive())
		return false;

	ImGui::Separator();

	ImGui::BeginChild(GetName().c_str(), GetSize());

	ImGui::PushID((UINT)m_componentType);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.5f, 0.8f, 1.f));
	ImGui::Button(ComponentTypeString[(UINT)m_componentType]);
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

void CComponentGUI::End()
{
	ImGui::EndChild();
}

void CComponentGUI::SubComponent()
{
	if (ImGui::MenuItem("Sub Component##ComponentMenu"))
	{
		m_target->SubComponent(m_componentType);
		Deactivate();
		m_bSubComponent = true;
	}
}
