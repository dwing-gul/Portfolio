#include "pch.h"
#include "CLayUpdateBoxGUI.h"

CLayUpdateBoxGUI::CLayUpdateBoxGUI() :
	CScriptGUI(SCRIPT_TYPE::LAYERUPDATEBOX)
{
	Activate();
}

CLayUpdateBoxGUI::~CLayUpdateBoxGUI()
{
}

void CLayUpdateBoxGUI::update()
{
	if (Start())
	{
		CLayerUpdateBox* layerUpdateBox = (CLayerUpdateBox*)GetTarget()->GetScriptComponent((UINT)SCRIPT_TYPE::LAYERUPDATEBOX);
		int layerNumberID = layerUpdateBox->GetLayerNumber();
		
		if (ImGui::InputInt("LayerUpdateNumber", &layerNumberID)) layerUpdateBox->SetLayerNumber(layerNumberID);

		End();
	}
}
