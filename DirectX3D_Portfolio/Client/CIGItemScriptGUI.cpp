#include "pch.h"
#include "CIGItemScriptGUI.h"


CIGItemScriptGUI::CIGItemScriptGUI() :
	CScriptGUI(SCRIPT_TYPE::IGITEMOBJSCRIPT)
{
	Activate();
}

CIGItemScriptGUI::~CIGItemScriptGUI()
{
}

void CIGItemScriptGUI::init()
{
}

void CIGItemScriptGUI::update()
{
	if (Start())
	{
		CIGItemObjScript* IgItemScript = (CIGItemObjScript*)GetTarget()->GetScriptComponent((UINT)SCRIPT_TYPE::IGITEMOBJSCRIPT);

		End();
	}
}
