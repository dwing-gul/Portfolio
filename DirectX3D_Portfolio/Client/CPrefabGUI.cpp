#include "pch.h"
#include "CPrefabGUI.h"
#include "CInspectorGUI.h"
#include "CImGuiManager.h"
#include <Engine/CPrefab.h>

CPrefabGUI::CPrefabGUI() :
	CResourceGUI(RESOURCE_TYPE::PREFAB)
{
}

CPrefabGUI::~CPrefabGUI()
{
}

void CPrefabGUI::update()
{
	if (Start())
	{
		CGameObject*& target = ((CPrefab*)GetTarget())->GetPrefabObject();
		CInspectorGUI* inspector = (CInspectorGUI*)CImGuiManager::GetInst()->FindGUI("Inspector");
		inspector->SetTargetObject(target);

		End();
	}
}
