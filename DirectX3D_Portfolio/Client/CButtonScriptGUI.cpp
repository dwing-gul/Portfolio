#include "pch.h"
#include "CButtonScriptGUI.h"

CButtonScriptGUI::CButtonScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::BUTTONUISCRIPT)
{
}

CButtonScriptGUI::~CButtonScriptGUI()
{
}

void CButtonScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		End();
	}
}
