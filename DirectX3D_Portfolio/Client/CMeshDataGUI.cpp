#include "pch.h"
#include "CMeshDataGUI.h"
#include <Engine/CMeshData.h>
#include <Engine/CTransform.h>
#include <Engine/CGameObject.h>
#include <Engine/CSceneManager.h>

CMeshDataGUI::CMeshDataGUI() :
	CResourceGUI(RESOURCE_TYPE::MESHDATA)
{
}

CMeshDataGUI::~CMeshDataGUI()
{
}

void CMeshDataGUI::update()
{
	if (Start())
	{
		CMeshData* meshdata = (CMeshData*)GetTarget();
		wstring name = meshdata->GetName();
		vector<CGameObject*> vecTarget = CSceneManager::GetInst()->FindObjByName_Vector(meshdata->GetName());
		vector<tFrameTransform>& vecTransInfo = meshdata->GetVectorTransInfo();

		for (size_t i = 0; i < vecTransInfo.size(); ++i)
		{
			string id = "Object Index ";
			char attach[255] = "";
			sprintf_s(attach, "%d##MeshData", (int)i);
			id += attach;

			if (ImGui::TreeNode(id.c_str()))
			{
				tFrameTransform trans = {};
				trans.translate = meshdata->GetTranslate((UINT)i);
				trans.scale = meshdata->GetScale((UINT)i);
				trans.rot = meshdata->GetRotation((UINT)i);
				trans.rot = (trans.rot * 180.f) / XM_PI;

				ImGui::Text("Position");
				ImGui::SameLine(70.f);
				ImGui::SetNextItemWidth(250);
				ImGui::InputFloat3("##Position", trans.translate);

				ImGui::Text("Scale   ");
				ImGui::SameLine(70.f);
				ImGui::SetNextItemWidth(250);
				ImGui::InputFloat3("##Scale", trans.scale);

				ImGui::Text("Rotation");
				ImGui::SameLine(70.f);
				ImGui::SetNextItemWidth(250);
				ImGui::InputFloat3("##Rotation", trans.rot);

				trans.rot = (trans.rot * XM_PI) / 180.f;

				meshdata->SetTransform((UINT)i, trans);

				for (size_t j = 0; j < vecTarget.size(); ++j)
				{
					int index = vecTarget[j]->GetMeshDataIndex();

					if (index == i)
					{
						vecTarget[j]->Transform()->SetLocalPos(trans.translate);
						vecTarget[j]->Transform()->SetLocalScale(trans.scale);
						vecTarget[j]->Transform()->SetLocalRot(trans.rot);
					}
				}

				ImGui::TreePop();
			}
		}

		ImVec2 buttonSize = ImVec2(50.f, 30.f);
		float space = (ImGui::GetWindowWidth() - buttonSize.x) / 2.f;
		ImGui::Dummy(ImVec2(0.f, 10.f));
		ImGui::Dummy(ImVec2(0.f, 0.f));
		ImGui::SameLine(space);
		if (ImGui::Button("Save##MeshData", buttonSize))
			meshdata->Save(meshdata->GetRelativePath());

		End();
	}
}
