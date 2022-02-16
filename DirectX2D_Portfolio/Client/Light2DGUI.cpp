#include "pch.h"
#include "Light2DGUI.h"
#include <Engine/Light2D.h>

Light2DGUI::Light2DGUI() :
	ComponentGUI(COMPONENT_TYPE::LIGHT2D),
	m_axis(Vector3(1.f, 0.f, 0.f)),
	m_bFirst(true)
{
}

Light2DGUI::~Light2DGUI()
{
}

void Light2DGUI::update()
{
	if (Start())
	{
		Light2D* light = GetTarget()->GetLight2D();

		tLightInfo& info = light->GetLightInfo();
		Vector3 pos = info.lightPos;

		if (m_bFirst)
		{
			m_axis = info.axis;
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

		ImGui::Text("Axis");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		if (ImGui::InputFloat2("##LightAxis", m_axis))
		{
			m_bFirst = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Select##Axis"))
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
			light->SetAxis(m_axis);
			bInput = false;
		}

		End();
	}
}