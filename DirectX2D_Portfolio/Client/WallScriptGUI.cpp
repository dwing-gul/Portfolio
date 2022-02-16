#include "pch.h"
#include "WallScriptGUI.h"
#include "ListGUI.h"
#include "ImGuiManager.h"
#include <Script/WallScript.h>

const char* WallTypeString[2] =
{
	"Right",
	"Left"
};

WallScriptGUI::WallScriptGUI() :
	ScriptGUI(SCRIPT_TYPE::WALLSCRIPT)
{
}

WallScriptGUI::~WallScriptGUI()
{
}

void WallScriptGUI::update()
{
	if (Start())
	{
		WALL_TYPE type = WALL_TYPE::RIGHT;
		type = ((WallScript*)GetScript())->GetWallType();

		ImGui::Text("WallType");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		int wallTypeidx = (int)type; // Here we store our selection data as an index.
		const char* combo_preview_value = WallTypeString[wallTypeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##WallType", combo_preview_value))
		{
			for (UINT i = 0; i < 2; i++)
			{
				const bool is_selected = (wallTypeidx == i);
				if (ImGui::Selectable(WallTypeString[i], is_selected))
				{
					wallTypeidx = i;
					type = (WALL_TYPE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		((WallScript*)GetScript())->SetWallType(type);

		End();
	}
}
