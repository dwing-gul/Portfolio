#include "pch.h"
#include "Collider2DGUI.h"
#include <Engine/Collider2D.h>

Collider2DGUI::Collider2DGUI() :
	ComponentGUI(COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DGUI::~Collider2DGUI()
{
}

void Collider2DGUI::update()
{
	if (Start())
	{
		Collider2D* collider = GetTarget()->GetCollider2D();

		Vector2 offsetPos = collider->GetOffsetPos();
		Vector2 offsetScale = collider->GetOffsetScale();

		ImGui::Text("Position");
		ImGui::SameLine(50.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat2("##OffsetPosition", offsetPos);

		ImGui::Text("Scale");
		ImGui::SameLine(50.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat2("##OffsetScale", offsetScale);

		collider->SetOffsetPos(offsetPos);
		collider->SetOffsetScale(offsetScale);

		End();
	}
}