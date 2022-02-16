#include "pch.h"
#include "CItemBoxScriptGUI.h"

CItemBoxScriptGUI::CItemBoxScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::ITEMBOXUISCRIPT)
{
}

CItemBoxScriptGUI::~CItemBoxScriptGUI()
{
}

void CItemBoxScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		End();
	}
}
