#include "pch.h"
#include "CMeshGUI.h"

CMeshGUI::CMeshGUI() :
	CResourceGUI(RESOURCE_TYPE::MESH)
{
}

CMeshGUI::~CMeshGUI()
{
}

void CMeshGUI::update()
{
	if (Start())
	{
		End();
	}
}