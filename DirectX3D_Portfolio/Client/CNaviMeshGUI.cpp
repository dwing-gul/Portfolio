#include "pch.h"
#include "CNaviMeshGUI.h"
#include <Engine/CNaviMesh.h>

CNaviMeshGUI::CNaviMeshGUI()
	:CComponentGUI(COMPONENT_TYPE::NAVIMESH)
{
}

CNaviMeshGUI::~CNaviMeshGUI()
{
}

void CNaviMeshGUI::update()
{
	if (Start())
	{
		CNaviMesh* naviMesh = GetTarget()->NaviMesh();
		bool bWireFrame = naviMesh->IsWireFrame();

		ImGui::Text("Wire Frame");
		ImGui::SameLine();
		ImGui::Checkbox("##WireFrame_NaviMesh", &bWireFrame);

		naviMesh->SetWireFrame(bWireFrame);

		End();
	}
}
