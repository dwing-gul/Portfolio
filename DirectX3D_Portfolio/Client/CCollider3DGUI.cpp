#include "pch.h"
#include "CCollider3DGUI.h"

#include <Engine\CCollider3D.h>

CCollider3DGUI::CCollider3DGUI()
	: CComponentGUI(COMPONENT_TYPE::COLLIDER3D)
{
}

CCollider3DGUI::~CCollider3DGUI()
{
}


void CCollider3DGUI::update()
{
	if (Start())
	{
		if (GetTarget())
		{
			CCollider3D* collider = GetTarget()->Collider3D();

			Vec3 offsetpos = collider->GetOffsetPos();
			Vec3 offsetscale = collider->GetOffsetScale();

			ImGui::PushItemWidth(200);
			ImGui::Text("Position"); ImGui::SameLine(); if (ImGui::InputFloat3("##ColPosition", (float*)&offsetpos)) collider->SetOffsetPos(offsetpos);
			ImGui::Text("Scale   "); ImGui::SameLine(); if (ImGui::InputFloat3("##ColScale", (float*)&offsetscale)) collider->SetOffsetScale(offsetscale);
			ImGui::PopItemWidth();
		}

		End();
	}
}
