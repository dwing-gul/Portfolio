#include "pch.h"
#include "CTransformGUI.h"
#include <Engine/CTransform.h>

CTransformGUI::CTransformGUI() :
	CComponentGUI(COMPONENT_TYPE::TRANSFORM)
{
}

CTransformGUI::~CTransformGUI()
{
}

void CTransformGUI::update()
{
	if (Start())
	{
		CTransform* transform = GetTarget()->Transform();

		Vector3 pos = transform->GetLocalPos();
		Vector3 scale = transform->GetLocalScale();
		Vector3 rot = transform->GetLocalRot();
		rot = (rot * 180.f) / XM_PI;

		ImGui::Text("Position"); 
		ImGui::SameLine(50.f); 
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##Position", pos);

		ImGui::Text("Scale   "); 
		ImGui::SameLine(50.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##Scale", scale);

		ImGui::Text("Rotation"); 
		ImGui::SameLine(50.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##Rotation", rot);

		transform->SetLocalPos(pos);
		transform->SetLocalScale(scale);

		rot = (rot * XM_PI) / 180.f;
		transform->SetLocalRot(rot);
		
		Vector3 worldPos = transform->GetWorldPos();
		Vector3 worldScale = transform->GetWorldScale();

		ImGui::Text("WorldPosition");
		ImGui::SameLine(100.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##WorldPosition", worldPos);

		ImGui::Text("WorldScale   ");
		ImGui::SameLine(100.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##WorldScale", worldScale);
		End();
	}
}
