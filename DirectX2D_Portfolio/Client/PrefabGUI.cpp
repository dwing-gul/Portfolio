#include "pch.h"
#include "PrefabGUI.h"
#include "InspectorGUI.h"
#include "ImGuiManager.h"
#include <Engine/Prefab.h>

PrefabGUI::PrefabGUI() :
	ResourceGUI(RESOURCE_TYPE::PREFAB)
{
}

PrefabGUI::~PrefabGUI()
{
}

void PrefabGUI::update()
{
	if (Start())
	{
		GameObject*& target = ((Prefab*)GetTarget())->GetPrefabObject();
		InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("Inspector");
		inspector->SetTargetObject(target);

		End();
	}
}
