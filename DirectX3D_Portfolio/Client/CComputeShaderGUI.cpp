#include "pch.h"
#include "CComputeShaderGUI.h"

CComputeShaderGUI::CComputeShaderGUI() :
	CShaderGUI(RESOURCE_TYPE::GRAPHICS)
{
}

CComputeShaderGUI::~CComputeShaderGUI()
{
}

void CComputeShaderGUI::update()
{
	if (Start())
	{
		End();
	}
}
