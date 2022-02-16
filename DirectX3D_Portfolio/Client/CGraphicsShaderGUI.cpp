#include "pch.h"
#include "CGraphicsShaderGUI.h"

CGraphicsShaderGUI::CGraphicsShaderGUI() :
	CShaderGUI(RESOURCE_TYPE::GRAPHICS)
{
}

CGraphicsShaderGUI::~CGraphicsShaderGUI()
{
}

void CGraphicsShaderGUI::update()
{
	if (Start())
	{
		End();
	}
}
