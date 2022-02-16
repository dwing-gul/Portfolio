#include "pch.h"
#include "CScriptGUI.h"
#include <Engine/CGameObject.h>

CScriptGUI::CScriptGUI(SCRIPT_TYPE _type) :
	CComponentGUI(COMPONENT_TYPE::SCRIPT),
	m_scriptType(_type),
	m_script(nullptr),
	m_bSubScript(false)
{
}

CScriptGUI::~CScriptGUI()
{
}

bool CScriptGUI::Start()
{
	if (nullptr != GetTarget() && GetTarget()->IsDead())
	{
		SetTarget(nullptr);
	}
	else if(nullptr != GetTarget() && !GetTarget()->IsDead())
	{
		const vector<CScript*>& vectorScript = GetTarget()->GetScripts();

		if (!vectorScript.empty())
		{
			for (size_t i = 0; i < vectorScript.size(); ++i)
			{
				if (vectorScript[i]->GetScriptType() != (UINT)m_scriptType)
				{
					Deactivate();
				}
				else
				{
					Activate();
					m_script = vectorScript[i];
					m_scriptName = WStringToString(CScriptManager::GetScriptName(m_script));
				}
			}
		}
		else
		{
			Deactivate();
		}
	}
	else
	{
		Deactivate();
	}

	if (!IsActive())
		return false;

	ImGui::Separator();

	ImGui::BeginChild(GetName().c_str(), GetSize());

	ImGui::PushID((UINT)m_scriptType);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.5f, 0.8f, 1.f));
	ImGui::Button(m_scriptName.c_str());
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		ImGui::OpenPopup("Scripts##Add Component");
	if (ImGui::BeginPopup("Scripts##Add Component"))
	{
		SubScript(m_scriptType);
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor(1);
	ImGui::PopID();

	if (m_bSubScript)
	{
		m_bSubScript = false;
		End();
		return false;
	}

	return true;
}

void CScriptGUI::SubScript(SCRIPT_TYPE _type)
{
	vector<CScript*>& vectorScript = GetTarget()->GetScripts_Ref();
	vector<CScript*>::iterator iter = vectorScript.begin();

	for (; iter != vectorScript.end();)
	{
		if ((*iter)->GetScriptType() == (UINT)_type)
		{
			iter = vectorScript.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	m_bSubScript = true;
}
