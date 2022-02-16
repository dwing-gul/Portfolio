#include "pch.h"
#include "CParticleSystemGUI.h"
#include "CListGUI.h"
#include "CImGuiManager.h"
#include <Engine/CParticleSystem.h>

CParticleSystemGUI::CParticleSystemGUI() :
	CComponentGUI(COMPONENT_TYPE::PARTICLE)
{
}

CParticleSystemGUI::~CParticleSystemGUI()
{
}

void CParticleSystemGUI::update()
{
	if (Start())
	{
		CParticleSystem* particle = GetTarget()->ParticleSystem();

		vector<Ptr<CTexture>>& vecTexture = particle->GetVectorTexture();
		UINT textureIndex = particle->GetTextureIndex();
		Vector4 startColor = particle->GetStartColor();
		Vector4 endColor = particle->GetEndColor();
		Vector3 startScale = particle->GetStartScale();
		Vector3 endScale = particle->GetEndScale();
		float startSpeed = particle->GetStartSpeed();
		float endSpeed = particle->GetEndSpeed();
		float minLifeTime = particle->GetMinLifeTime();
		float maxLifeTime = particle->GetMaxLifeTime();
		UINT spawnRate = particle->GetSpawnRate();
		UINT maxParticleCount = particle->GetMaxParticleCount();
		Vector3 area = particle->GetArea();

		char textureName[255] = {};
		if(!vecTexture.empty())
			GetResourceName(vecTexture[textureIndex].Get(), textureName);

		ImGui::Text("Texture");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Texture_Particle", textureName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Texture_Particle"))
		{
			CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Texture_Particle");
			vector<wstring> vecName;
			GetTextureName(vecName);
			listGUI->SetList(vecName);
			listGUI->SetDoubleClickedCallback(this, (GUI_CALLBACK)&CParticleSystemGUI::SetTexture);
			listGUI->Activate();
		}

		if (ImGui::TreeNodeEx("StartColor"))
		{
			static bool alpha_start = true;
			static bool alpha_bar_start = true;
			static bool side_preview_start = true;
			static bool ref_color_start = false;
			static ImVec4 ref_color_v_start(1.0f, 0.f, 1.0f, 0.5f);
			static int display_mode_start = 0;
			static int picker_mode_start = 0;
			ImGuiColorEditFlags flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreviewHalf;

			ImGui::Checkbox("With Alpha##StartColor", &alpha_start);
			ImGui::Checkbox("With Alpha Bar##StartColor", &alpha_bar_start);
			ImGui::Checkbox("With Side Preview##StartColor", &side_preview_start);
			if (side_preview_start)
			{
				ImGui::SameLine();
				ImGui::Checkbox("With Ref Color##StartColor", &ref_color_start);
				if (ref_color_start)
				{
					ImGui::SameLine();
					ImGui::ColorEdit4("##RefColor_StartColor", &ref_color_v_start.x, ImGuiColorEditFlags_NoInputs | flags);
				}
			}
			ImGui::Combo("Display Mode##StartColor", &display_mode_start, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
			ImGui::Combo("Picker Mode##StartColor", &picker_mode_start, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
			if (!alpha_start)            flags |= ImGuiColorEditFlags_NoAlpha;
			if (alpha_bar_start)         flags |= ImGuiColorEditFlags_AlphaBar;
			if (!side_preview_start)     flags |= ImGuiColorEditFlags_NoSidePreview;
			if (picker_mode_start == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
			if (picker_mode_start == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
			if (display_mode_start == 1) flags |= ImGuiColorEditFlags_NoInputs;
			if (display_mode_start == 2) flags |= ImGuiColorEditFlags_DisplayRGB;
			if (display_mode_start == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
			if (display_mode_start == 4) flags |= ImGuiColorEditFlags_DisplayHex;
			ImGui::ColorPicker4("##StartColor", startColor, flags, ref_color_start ? &ref_color_v_start.x : NULL);

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("EndColor"))
		{
			static bool alpha_end = true;
			static bool alpha_bar_end = true;
			static bool side_preview_end = true;
			static bool ref_color_end = false;
			static ImVec4 ref_color_v_end(1.0f, 0.f, 1.0f, 0.5f);
			static int display_mode_end = 0;
			static int picker_mode_end = 0;
			ImGuiColorEditFlags flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreviewHalf;

			ImGui::Checkbox("With Alpha##EndColor", &alpha_end);
			ImGui::Checkbox("With Alpha Bar##EndColor", &alpha_bar_end);
			ImGui::Checkbox("With Side Preview##EndColor", &side_preview_end);
			if (side_preview_end)
			{
				ImGui::SameLine();
				ImGui::Checkbox("With Ref Color##EndColor", &ref_color_end);
				if (ref_color_end)
				{
					ImGui::SameLine();
					ImGui::ColorEdit4("##RefColor_EndColor", &ref_color_v_end.x, ImGuiColorEditFlags_NoInputs | flags);
				}
			}
			ImGui::Combo("Display Mode##EndColor", &display_mode_end, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
			ImGui::Combo("Picker Mode##EndColor", &picker_mode_end, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
			if (!alpha_end)            flags |= ImGuiColorEditFlags_NoAlpha;
			if (alpha_bar_end)         flags |= ImGuiColorEditFlags_AlphaBar;
			if (!side_preview_end)     flags |= ImGuiColorEditFlags_NoSidePreview;
			if (picker_mode_end == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
			if (picker_mode_end == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
			if (display_mode_end == 1) flags |= ImGuiColorEditFlags_NoInputs;
			if (display_mode_end == 2) flags |= ImGuiColorEditFlags_DisplayRGB;
			if (display_mode_end == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
			if (display_mode_end == 4) flags |= ImGuiColorEditFlags_DisplayHex;
			ImGui::ColorPicker4("##EndColor", endColor, flags, ref_color_end ? &ref_color_v_end.x : NULL);
			ImGui::TreePop();
		}

		ImGui::Text("StartScale");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##StartScale", startScale);

		ImGui::Text("EndScale");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##EndScale", endScale);

		ImGui::Text("StartSpeed");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##StartSpeed", &startSpeed);

		ImGui::Text("EndSpeed");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##EndSpeed", &endSpeed);

		ImGui::Text("LifeTime");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::DragFloatRange2("##LifeTime", &minLifeTime, &maxLifeTime, 0.25f, 0.f, 100.f, "Min: %.1f sec", "Max: %.1f sec", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("SpawnRate");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##SpawnRate", (int*)&spawnRate);

		ImGui::Text("MaxParticleCount");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##MaxParticleCount", (int*)&maxParticleCount);

		ImGui::Text("Area");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat3("##Area", area);

		particle->SetStartColor(startColor);
		particle->SetEndColor(endColor);
		particle->SetStartScale(startScale);
		particle->SetEndScale(endScale);
		particle->SetStartSpeed(startSpeed);
		particle->SetEndSpeed(endSpeed);
		particle->SetMinLifeTime(minLifeTime);
		particle->SetMaxLifeTime(maxLifeTime);
		particle->SetSpawnRate(spawnRate);
		particle->SetMaxParticleCount(maxParticleCount);
		particle->SetArea(area);

		End();
	}
}

void CParticleSystemGUI::SetTexture(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = StringToWString(name);
	CParticleSystem* particle = GetTarget()->ParticleSystem();
	UINT index = 0;

	map<wstring, CRes*> mapTexture = CResourceManager::GetInst()->GetRes(RESOURCE_TYPE::TEXTURE);

	map<wstring, CRes*>::iterator iter = mapTexture.find(key);
	if (iter == mapTexture.end())
		return;

	vector<Ptr<CTexture>>& vectorTexture = particle->GetVectorTexture();
	vectorTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(iter->first, iter->second->GetRelativePath()));

	for (size_t i = 0; i < vectorTexture.size(); ++i)
	{
		if (key == vectorTexture[i]->GetKey())
			index = (UINT)i;
	}

	particle->SetTextureIndex(index);
}

void CParticleSystemGUI::GetTextureName(vector<wstring>& _vectorName)
{
	map<wstring, CRes*> mapTexture = CResourceManager::GetInst()->GetRes(RESOURCE_TYPE::TEXTURE);

	map<wstring, CRes*>::iterator iter = mapTexture.begin();
	for (; iter != mapTexture.end(); ++iter)
	{
		_vectorName.push_back(iter->first);
	}
}
