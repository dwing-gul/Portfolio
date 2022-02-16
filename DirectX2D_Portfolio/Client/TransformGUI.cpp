#include "pch.h"
#include "TransformGUI.h"
#include <Engine/Transform.h>

TransformGUI::TransformGUI() :
	ComponentGUI(COMPONENT_TYPE::TRANSFORM)
{
}

TransformGUI::~TransformGUI()
{
}

void TransformGUI::update()
{
	if (Start())
	{
		Transform* transform = GetTarget()->GetTransform();

		Vector3 pos = transform->GetLocalPos();
		Vector3 scale = transform->GetLocalScale();
		Vector3 rot = transform->GetLocalRot();
		rot = (rot * 180.f) / XM_PI;

		ImGui::Text("Position");
		ImGui::SameLine(50.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##Position", pos);

		ImGui::Text("Scale");
		ImGui::SameLine(50.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##Scale", scale);

		ImGui::Text("Rotation");
		ImGui::SameLine(50.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##Rotation", rot);

		transform->SetLocalPos(pos);
		transform->SetLocalScale(scale);

		rot = (rot * XM_PI) / 180.f;
		transform->SetLocalRot(rot);

		End();
	}
}