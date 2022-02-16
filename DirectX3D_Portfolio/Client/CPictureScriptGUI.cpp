#include "pch.h"
#include "CPictureScriptGUI.h"

CPictureScriptGUI::CPictureScriptGUI() :
	CUIScriptGUI(SCRIPT_TYPE::PICTUREUISCRIPT)
{
}

CPictureScriptGUI::~CPictureScriptGUI()
{
}

void CPictureScriptGUI::update()
{
	if (Start())
	{
		CUIScriptGUI::SettingUIType();

		End();
	}
}
