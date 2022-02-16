#include "pch.h"
#include "DoorScriptGUI.h"
#include "ListGUI.h"
#include "ImGuiManager.h"
#include <Script/DoorScript.h>
#include <Engine/GameObject.h>
#include <Engine/SceneManager.h>

const char* DoorTypeString[(UINT)DOOR_TYPE::END] =
{
	"Enterance",
	"Exit",
	"RoomEnterance",
	"RoomExit",
};

DoorScriptGUI::DoorScriptGUI() :
	ScriptGUI(SCRIPT_TYPE::DOORSCRIPT)
{
}

DoorScriptGUI::~DoorScriptGUI()
{
}

void DoorScriptGUI::update()
{
	if (Start())
	{
		DOOR_TYPE type = ((DoorScript*)GetScript())->GetDoorType();
		GameObject* dest = ((DoorScript*)GetScript())->GetDestination();
		const wstring& name = ((DoorScript*)GetScript())->GetDestName();

		char destName[255] = {};
		string temp = {};
		temp = WStringToString(name);

		strcpy_s(destName, temp.c_str());

		ImGui::Text("DoorType");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		int doorTypeidx = (int)type; // Here we store our selection data as an index.
		const char* combo_preview_value = DoorTypeString[doorTypeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##DoorType", combo_preview_value))
		{
			for (UINT i = 0; i < (UINT)DOOR_TYPE::END; i++)
			{
				const bool is_selected = (doorTypeidx == i);
				if (ImGui::Selectable(DoorTypeString[i], is_selected))
				{
					doorTypeidx = i;
					type = (DOOR_TYPE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		((DoorScript*)GetScript())->SetDoorType(type);

		ImGui::Text("Destination");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##DoorScript", destName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##DoorScript"))
		{
			ListGUI* listGUI = (ListGUI*)ImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Destination");
			vector<wstring> vectorName;
			const vector<GameObject*> vectorInteraction = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::INTERACTION);

			for (size_t i = 0; i < vectorInteraction.size(); ++i)
			{
				vectorName.push_back(vectorInteraction[i]->GetName());
			}

			listGUI->SetList(vectorName);
			listGUI->SetDoubleClickedCallBack(this, (GUI_CALLBACK)&DoorScriptGUI::SetDestination);
			listGUI->Activate();
		}


		End();
	}
}

void DoorScriptGUI::SetDestination(DWORD_PTR _dw1, DWORD_PTR _dw2)
{
	string name = (const char*)_dw1;
	wstring destName = StringToWString(name);
	GameObject* dest = SceneManager::GetInst()->FindObjByName(destName.c_str());

	((DoorScript*)GetScript())->SetDestination(dest);
	((DoorScript*)GetScript())->SetDestName(destName);
}
