#include "pch.h"
#include "CTrackingUIScriptGUI.h"

CTrackingUIScriptGUI::CTrackingUIScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::TRACKINGUISCRIPT)
{
}

CTrackingUIScriptGUI::~CTrackingUIScriptGUI()
{
}

void CTrackingUIScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		End();
	}
}
