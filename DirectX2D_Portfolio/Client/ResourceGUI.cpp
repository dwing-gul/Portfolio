#include "pch.h"
#include "ResourceGUI.h"

ResourceGUI::ResourceGUI(RESOURCE_TYPE _type) :
	m_target(nullptr),
	m_type(_type)
{
	SetName(ResourceTypeString[(UINT)m_type]);
}

ResourceGUI::~ResourceGUI()
{
}

bool ResourceGUI::Start()
{
	if (nullptr == m_target)
	{
		Deactivate();
		return false;
	}

	if (!IsActive())
	{
		return false;
	}

	ImGui::BeginChild(GetName().c_str(), GetSize());

	ImGui::PushID((UINT)m_type);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.5f, 0.8f, 1.f));
	ImGui::Button(ResourceTypeString[(UINT)m_type]);
	ImGui::PopStyleColor(1);
	ImGui::PopID();

    return true;
}

void ResourceGUI::End()
{
	ImGui::EndChild();
}