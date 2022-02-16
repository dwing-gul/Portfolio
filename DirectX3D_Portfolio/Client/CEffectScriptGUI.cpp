#include "pch.h"
#include "CEffectScriptGUI.h"

CEffectScriptGUI::CEffectScriptGUI():
	CScriptGUI(SCRIPT_TYPE::EFFECTSCRIPT)
{
	Activate();
}

CEffectScriptGUI::~CEffectScriptGUI()
{
}

void CEffectScriptGUI::init()
{
}
void CEffectScriptGUI::update()
{
	if (Start())
	{
		CEffectScript* effectScript = (CEffectScript*)GetTarget()->GetScriptComponent((UINT)SCRIPT_TYPE::EFFECTSCRIPT);

		bool bDecal = effectScript->GetDecal();

		if (ImGui::Checkbox("Decal", &bDecal)) { effectScript->SetDecalEffect(bDecal); }

		if (!bDecal)
		{
			Vec3 color = effectScript->GetEffectColor();

			if (ImGui::ColorEdit3("Effect Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
				effectScript->SetEffectColor(color);
		}
		
		else
		{
			Vec3 color = effectScript->GetEffectColor();
			int mode = effectScript->GetMode();
			float ratio = effectScript->GetRatio();


			if (ImGui::ColorEdit3("Effect Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
				effectScript->SetEffectColor(color);

			if (ImGui::InputInt("Mode", &mode))
				effectScript->SetMode(mode);

			if (ImGui::InputFloat("Ratio", &ratio))
				effectScript->SetRatio(ratio);
		}


		End();
	}
}

