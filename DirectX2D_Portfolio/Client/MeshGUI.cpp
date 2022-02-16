#include "pch.h"
#include "MeshGUI.h"

MeshGUI::MeshGUI() :
	ResourceGUI(RESOURCE_TYPE::MESH)
{
}

MeshGUI::~MeshGUI()
{
}

void MeshGUI::update()
{
	if (Start())
	{
		End();
	}
}