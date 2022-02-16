#include "pch.h"
#include "CLight2DGUI.h"
#include <Engine/CLight2D.h>

CLight2DGUI::CLight2DGUI() :
	CComponentGUI(COMPONENT_TYPE::LIGHT2D),
	m_dir(Vector3(1.f, 0.f, 0.f)),
	m_bFirst(true)
{
}

CLight2DGUI::~CLight2DGUI()
{
}

void CLight2DGUI::update()
{
	if (Start())
	{
		CLight2D* light = GetTarget()->Light2D();

		tLightInfo& info = light->GetLightInfo();
		Vector3 pos = info.lightPos;

		if (m_bFirst)
		{
			m_dir = info.lightDir;
		}

		bool bInput = false;
		float angle = info.angle;
		angle = (angle * 360.f) / XM_PI;

		ImGui::Text("Diffuse");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##LightDiffuse", info.color.diffuse);

		ImGui::Text("Specular");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##LightSpecular", info.color.specular);

		ImGui::Text("Ambient");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##LightAmbient", info.color.ambient);

		ImGui::Text("Position");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##LightPosition", pos);

		ImGui::Text("Direction");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		if (ImGui::InputFloat2("##LightDirection", m_dir))
		{
			m_bFirst = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Select##Direction"))
		{
			bInput = true;
			m_bFirst = true;
		}

		ImGui::Text("Angle");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##LightAngle", &angle);

		ImGui::Text("Range");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##LightRange", &info.range);

		ImGui::Text("LightType");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
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
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		light->SetLightPos(pos);
		light->SetAngle(angle);

		if (bInput)
		{
			light->SetDir(Vec4(m_dir, 0.f));
			bInput = false;
		}

		End();
	}
}