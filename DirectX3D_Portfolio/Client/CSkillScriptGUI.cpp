#include "pch.h"
#include "CSkillScriptGUI.h"

CSkillScriptGUI::CSkillScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::SKILLUISCRIPT)
{
}

CSkillScriptGUI::~CSkillScriptGUI()
{
}

void CSkillScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		End();
	}
}
