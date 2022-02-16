#include "pch.h"
#include "ComputeShaderGUI.h"

ComputeShaderGUI::ComputeShaderGUI() :
	ShaderGUI(RESOURCE_TYPE::GRAPHICS)
{
}

ComputeShaderGUI::~ComputeShaderGUI()
{
}

void ComputeShaderGUI::update()
{
	if (Start())
	{
		End();
	}
}
