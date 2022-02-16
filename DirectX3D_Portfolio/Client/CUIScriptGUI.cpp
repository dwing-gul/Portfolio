#include "pch.h"
#include "CUIScriptGUI.h"
#include <Script/CUIScript.h>

const char* UITypeString[(UINT)UI_TYPE::END] =
{
	 "Default",
	 "Profile",
	 "Weapon_Icon",
	 "Skill_Image",
	 "Weapon_Skill_Image",
	 "Skill_Level",
	 "Font_Fix",
	 "Font_Changeable",
	 "Button_Skill",
	 "Button_Passive_Skill",
	 "Button_None_Day",
	 "Button",
	 "Item_Box",
	 "Hp_Bar",
	 "Profile_Char"
};

CUIScriptGUI::CUIScriptGUI(SCRIPT_TYPE _type) :
	CScriptGUI(_type)
{
}

CUIScriptGUI::~CUIScriptGUI()
{
}

void CUIScriptGUI::SettingUIType()
{
	CUIScript* uiScript = (CUIScript*)GetTarget()->GetScripts()[0];
	INT_PTR id = (INT_PTR)GetTarget();
	string stringID;

	UI_TYPE uiType = uiScript->GetUIType();

	char num[255];
	sprintf_s(num, "##UIType%lld", id);
	stringID = num;

	ImGui::Text("UI Type");
	ImGui::SameLine(100.f, 5.f);
	ImGui::SetNextItemWidth(180);
	int uiTypeidx = (int)uiType; // Here we store our selection data as an index.
	const char* combo_preview_value_font = UITypeString[uiTypeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo(stringID.c_str(), combo_preview_value_font))
	{
		for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
		{
			const bool is_selected = (uiTypeidx == i);
			if (ImGui::Selectable(UITypeString[i], is_selected))
			{
				uiTypeidx = i;
				uiType = (UI_TYPE)i;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	uiScript->SetUIType(uiType);
}
