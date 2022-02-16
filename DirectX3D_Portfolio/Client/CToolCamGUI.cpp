#include "pch.h"
#include "CToolCamGUI.h"

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine\CRenderManager.h>

CToolCamGUI::CToolCamGUI()
{
	SetName("ToolCamGUI");
	Deactivate();
}

CToolCamGUI::~CToolCamGUI()
{
}

void CToolCamGUI::update()
{
	ImGui::Begin(GetName().c_str(), &IsCancelButton());

	if (!IsCancelButton())
	{
		Deactivate();
	}

	CCamera* toolCam = CRenderManager::GetInst()->GetToolCam();

	if (nullptr != toolCam)
	{
		CTransform* transform = toolCam->Transform();

		Vector3 pos = transform->GetLocalPos();
		Vector3 rot = transform->GetLocalRot();
		rot = (rot * 180.f) / XM_PI;

		ImGui::Text("Position"); ImGui::SameLine(100.f);	ImGui::SetNextItemWidth(180);
		if (ImGui::InputFloat3("##Position", pos)) { transform->SetLocalPos(pos); }

		ImGui::Text("Rotation"); ImGui::SameLine(100.f);	ImGui::SetNextItemWidth(180);
		if (ImGui::InputFloat3("##Rotation", rot)) { transform->SetLocalRot((rot * XM_PI) / 180.f); }
		
		Vector3 toolPrevPos = toolCam->GetPrevPos();
		Vector3 toolPrevRot = toolCam->GetPrevRot();

		ImGui::Text("PrevPosition"); ImGui::SameLine(100.f);	ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##PrevPosition", toolPrevPos);

		ImGui::Text("PrevRotation"); ImGui::SameLine(100.f);	ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##PrevRotation", toolPrevRot);

		if(ImGui::Button("GoBack")) { transform->SetLocalPos(toolPrevPos); transform->SetLocalRot(toolPrevRot); }
	}
	

	ImGui::End();
}
