#include "pch.h"
#include "CCollider2DGUI.h"
#include <Engine/CCollider2D.h>

CCollider2DGUI::CCollider2DGUI() :
	CComponentGUI(COMPONENT_TYPE::COLLIDER2D)
{
}

CCollider2DGUI::~CCollider2DGUI()
{
}

void CCollider2DGUI::update()
{
	if (Start())
	{
		CCollider2D* collider = GetTarget()->Collider2D();

		Vec2 offsetPos = collider->GetOffsetPos();
		Vec2 offsetScale = collider->GetOffsetScale();

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