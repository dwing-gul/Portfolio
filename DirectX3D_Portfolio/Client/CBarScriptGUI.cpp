#include "pch.h"
#include "CBarScriptGUI.h"

CBarScriptGUI::CBarScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::BARUISCRIPT)
{
}

CBarScriptGUI::~CBarScriptGUI()
{
}

void CBarScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		End();
	}
}
