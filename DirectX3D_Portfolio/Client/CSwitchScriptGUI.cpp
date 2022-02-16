#include "pch.h"
#include "CSwitchScriptGUI.h"

CSwitchScriptGUI::CSwitchScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::SWITCHUISCRIPT)
{
}

CSwitchScriptGUI::~CSwitchScriptGUI()
{
}

void CSwitchScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		End();
	}
}
