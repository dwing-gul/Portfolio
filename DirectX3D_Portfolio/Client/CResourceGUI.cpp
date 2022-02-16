#include "pch.h"
#include "CResourceGUI.h"

CResourceGUI::CResourceGUI(RESOURCE_TYPE _type) :
	m_target(nullptr),
	m_resType(_type)
{
	SetName(ResourceTypeString[(UINT)m_resType]);
}

CResourceGUI::~CResourceGUI()
{
}

bool CResourceGUI::Start()
{
	if (nullptr == m_target)
	{
		Deactivate();
		return false;
	}

	if (!IsActive())
		return false;

	ImGui::BeginChild(GetName().c_str(), GetSize());

	ImGui::PushID((UINT)m_resType);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.6f, 0.8f, 1.f));
	ImGui::Button(ResourceTypeString[(UINT)m_resType]);
	ImGui::PopStyleColor(1);
	ImGui::PopID();

	return true;
}

void CResourceGUI::End()
{
	ImGui::EndChild();
}
