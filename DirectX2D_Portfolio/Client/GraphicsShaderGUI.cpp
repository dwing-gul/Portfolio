#include "pch.h"
#include "GraphicsShaderGUI.h"

GraphicsShaderGUI::GraphicsShaderGUI() :
	ShaderGUI(RESOURCE_TYPE::GRAPHICS)
{
}

GraphicsShaderGUI::~GraphicsShaderGUI()
{
}

void GraphicsShaderGUI::update()
{
	if (Start())
	{
		End();
	}
}
