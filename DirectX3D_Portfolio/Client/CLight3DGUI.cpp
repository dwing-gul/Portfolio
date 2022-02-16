#include "pch.h"
#include "CLight3DGUI.h"
#include <Engine/CLight3D.h>

CLight3DGUI::CLight3DGUI():
	CComponentGUI(COMPONENT_TYPE::LIGHT3D),
	m_dir(Vector3(1.f, 0.f, 0.f)),
	m_bFirst(true)
{
}

CLight3DGUI::~CLight3DGUI()
{
}

void CLight3DGUI::update()
{
	if (Start())
	{
		CLight3D* light = GetTarget()->Light3D();

		tLightInfo info = light->GetLightInfo();
		Vector3 pos = info.lightPos;

		if (m_bFirst)
		{
			m_dir = info.lightDir;
		}

		bool bInput = false;
		float angle = info.angle;
		angle = (angle * 360.f) / XM_PI;

		ImGui::Text("Diffuse    "); ImGui::SameLine(); if (ImGui::ColorEdit4("Diff", (float*)&info.color.diffuse
			, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) light->SetDiffuseColor(info.color.diffuse);
		ImGui::Text("Specular "); ImGui::SameLine(); if (ImGui::ColorEdit4("Spec", (float*)&info.color.specular
			, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) light->SetSpecularColor(info.color.specular);
		ImGui::Text("Ambient  "); ImGui::SameLine(); if (ImGui::ColorEdit4("Amb ", (float*)&info.color.ambient
			, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) light->SetAmbientColor(info.color.ambient);

		ImGui::PushItemWidth(100);
		ImGui::Text("Range    "); ImGui::SameLine(); if (ImGui::InputFloat("##Range", &info.range)) light->SetRange(info.range);
		ImGui::Text("Angle     "); ImGui::SameLine(); if (ImGui::InputFloat("##Angle", &angle)) light->SetAngle(angle);
		ImGui::PopItemWidth();

		ImGui::Text("LightType");	
		int lightTypeidx = (int)info.lightType;
		const char* combo_preview_value = LightTypeString[lightTypeidx];
		if (ImGui::BeginCombo("##LightType", combo_preview_value))
		{
			for (UINT i = 0; i < (UINT)LIGHT_TYPE::END; i++)
			{
				const bool is_selected = (lightTypeidx == i);
				if (ImGui::Selectable(LightTypeString[i], is_selected))
				{
					lightTypeidx = i;
					info.lightType = (LIGHT_TYPE)i;
					light->SetLightType(info.lightType);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		End();
	}
}


