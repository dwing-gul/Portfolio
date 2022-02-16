#include "pch.h"
#include "CAIPointScriptGUI.h"

#include <Engine\CRenderManager.h>
#include <Engine\CNaviMesh.h>

CAIPointScriptGUI::CAIPointScriptGUI() :
	CScriptGUI(SCRIPT_TYPE::AIPOINTSCRIPT)
{
	Activate();
}

CAIPointScriptGUI::~CAIPointScriptGUI()
{
}

void CAIPointScriptGUI::init()
{
	m_pointType[0] = "MOVE";
	m_pointType[1] = "LOOTING";
	m_pointType[2] = "HUNTING";
}

void CAIPointScriptGUI::update()
{
	if (Start())
	{
		CAIPointScript* aiPointScript = (CAIPointScript*)GetTarget()->GetScriptComponent((UINT)SCRIPT_TYPE::AIPOINTSCRIPT);
		vector<tPointType>& vecPoint = aiPointScript->GetVecPoint();
		int vecSize = (int)vecPoint.size();
		static int curID = 0;

		ImGui::Text("vecsize");
		if (ImGui::InputInt("##vecSize", &vecSize))
		{
			if (vecPoint.size() > vecSize)
			{
				vecPoint.resize(vecSize);

				if (curID >= vecSize)
					curID = vecSize - 1;
			}
			else if (vecPoint.size() < vecSize)
			{
				for (int i = (int)vecPoint.size(); i < vecSize; ++i)
				{
					tPointType tType = {};
					tType.m_pointID = i;
					tType.m_pointType = POINT_TYPE::MOVE;
					vecPoint.push_back(tType);
				}
			}
		}

		if (!vecPoint.empty())
		{
			ImGui::Text("CurID");
			if (ImGui::InputInt("##CurID", &curID))
			{
				if (curID < 0) 
					curID = 0;
				else if (curID == vecPoint.size()) 
					curID = (int)vecPoint.size() - 1;
			}
			
			tPointType pointTypeStruct = aiPointScript->GetPointTypeStruct(curID);
			ImGui::Text("PointID");
			ImGui::InputInt("##PointID", &pointTypeStruct.m_pointID);

			if (KEY_HOLD(KEY::T) && KEY_TAP(KEY::LBUTTON) && CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PAUSE)
			{
				CGameObject* naviObj = CSceneManager::GetInst()->FindObjByName(L"NaviMesh");
				if (nullptr != naviObj && nullptr != naviObj->NaviMesh())
				{
					Vec3 Pos = naviObj->NaviMesh()->GetMouseClickPos(CRenderManager::GetInst()->GetToolCam());
					pointTypeStruct.m_pointPos = Pos;
					aiPointScript->SetPointPos(pointTypeStruct.m_pointPos, curID);
				}
				
				
			}

			ImGui::Text("PointPos");
			if (ImGui::InputFloat3("##PointPos", (float*)pointTypeStruct.m_pointPos))
			{
				aiPointScript->SetPointPos(pointTypeStruct.m_pointPos, curID);
			}

			GetTarget()->Transform()->SetLocalPos(pointTypeStruct.m_pointPos);

			ImGui::Text("PointType");
			
			POINT_TYPE pointType = pointTypeStruct.m_pointType;
			int curPointType = (int)pointType;

			const char* combo_preview_value = m_pointType[curPointType].c_str();
			if (ImGui::BeginCombo("##PointType", combo_preview_value))
			{
				for (UINT i = 0; i < (UINT)POINT_TYPE::HUNTING + 1; ++i)
				{
					const bool is_selected = (curPointType == i);
					if (ImGui::Selectable(m_pointType[i].c_str(), is_selected))
					{
						aiPointScript->SetPointType((POINT_TYPE)i, curID);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		

		End();
	}
}
