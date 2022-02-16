#include "pch.h"
#include "CCameraGUI.h"
#include <Engine/CCamera.h>
#include <Engine/CKeyManager.h>

CCameraGUI::CCameraGUI() :
	CComponentGUI(COMPONENT_TYPE::CAMERA)
{
}

CCameraGUI::~CCameraGUI()
{
}

void CCameraGUI::update()
{
	if (Start())
	{
		CCamera* camera = GetTarget()->Camera();

		PROJECTION_TYPE type = camera->GetProjectionType();
		float Far = camera->GetFar();
		float FOV = camera->GetFOV();
		float scale = camera->GetScale();

		ImGui::Text("ProjectionType");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		int projectionTypeidx = (int)type; // Here we store our selection data as an index.
		const char* combo_preview_value = ProjectionTypeString[projectionTypeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##ProjectionType", combo_preview_value))
		{
			for (UINT i = 0; i < (UINT)PROJECTION_TYPE::END; i++)
			{
				const bool is_selected = (projectionTypeidx == i);
				if (ImGui::Selectable(ProjectionTypeString[i], is_selected))
				{
					projectionTypeidx = i;
					type = (PROJECTION_TYPE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		camera->SetProjectionType(type);

		ImGui::Text("Far");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##Far", &Far);
		
		if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
		{
			camera->SetFar(Far);
		}

		ImGui::Text("FOV");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##FOV", &FOV);
		
		if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
		{
			camera->SetFOV(FOV);
		}

		ImGui::Text("Scale");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##Scale", &scale);

		if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
		{
			camera->SetScale(scale);
		}
		
		End();
	}
}
