#include "pch.h"
#include "CTerrainGUI.h"
#include "CListGUI.h"
#include "CImGuiManager.h"
#include <Engine/CTerrain.h>
#include <Engine/CDevice.h>

CTerrainGUI::CTerrainGUI() :
	CComponentGUI(COMPONENT_TYPE::TERRAIN)
{
}

CTerrainGUI::~CTerrainGUI()
{
}

void CTerrainGUI::update()
{
	if (Start())
	{
		CTerrain* terrain = GetTarget()->Terrain();

		// common
		// face count
		int face[2] = { (int)terrain->GetFaceCount()[0], (int)terrain->GetFaceCount()[1] };
		// terrain mode - none, height(up, down), splat
		TERRAIN_MODE mode = terrain->GetTerrainMode();
		// wireframe
		bool bWireFrame = terrain->IsWireFrame();
		// brush scale
		Vec2 brushScale = terrain->GetBrushScale();
		float tessLevel = terrain->GetMaxTessLevel();
		Vec2 tessDistMinMax = terrain->GetTessDistMinMax();
		int weightIndex = terrain->GetWeightIndex();
		int brushIndex = terrain->GetBrushIndex();
		Ptr<CTexture> brushArrTex = terrain->GetBrushArrTex();
		Ptr<CTexture> tileArrTex = terrain->GetTileArrTex();

		ImGui::Text("Wire Frame");
		ImGui::SameLine();
		ImGui::Checkbox("##WireFrame", &bWireFrame);

		ImGui::Text("Terrain Face Count");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::DragInt2("##TerrainFaceCount", face);

		ImGui::Text("Tessellation Max Level");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat("##TessellationMaxLevel", &tessLevel);

		ImGui::Text("Tessellation Distance Min Max");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputFloat2("##TessellationDistanceMinMax", tessDistMinMax);

		ImGui::Text("Terrain Mode");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		static int terrainModeidx = (int)mode; // Here we store our selection data as an index.
		const char* combo_preview_value = TerrainModeString[terrainModeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##ProjectionType", combo_preview_value))
		{
			for (UINT i = 0; i < (UINT)TERRAIN_MODE::END; i++)
			{
				const bool is_selected = (terrainModeidx == i);
				if (ImGui::Selectable(TerrainModeString[i], is_selected))
				{
					terrainModeidx = i;
					mode = (TERRAIN_MODE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		char brushTextureName[255] = {};
		GetResourceName(brushArrTex.Get(), brushTextureName);

		ImGui::Text("BrushTexture");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Brush_Texture_Terrain", brushTextureName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Brush_Texture_Terrain"))
		{
			CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Brush_Texture_Terrain");
			vector<wstring> vecName;
			CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vecName);
			listGUI->SetList(vecName);
			listGUI->SetDoubleClickedCallback(this, (GUI_CALLBACK)&CTerrainGUI::SetBrushTexture);
			listGUI->Activate();
		}

		ImGui::Text("Brush Index");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##BrushIndex", &brushIndex);

		ImGui::Text("Brush Scale");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::DragFloat2("##BrushScale", brushScale, 0.1f, 0.f, 1.f);

		char tileTextureName[255] = {};
		GetResourceName(tileArrTex.Get(), tileTextureName);

		ImGui::Text("TileTexture");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Tile_Texture_Terrain", tileTextureName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Tile_Texture_Terrain"))
		{
			CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Tile_Texture_Terrain");
			vector<wstring> vecName;
			CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vecName);
			listGUI->SetList(vecName);
			listGUI->SetDoubleClickedCallback(this, (GUI_CALLBACK)&CTerrainGUI::SetTileTexture);
			listGUI->Activate();
		}

		ImGui::Text("Terrain Tile Weight Index");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt("##TerrainTileWeightIndex", &weightIndex);

		terrain->SetFaceCount(face[0], face[1]);
		terrain->SetTerrainMode(mode);
		terrain->SetBrushScale(brushScale);
		terrain->SetWireFrame(bWireFrame);
		terrain->SetMaxTessLevel(tessLevel);
		terrain->SetTessDistMinMax(tessDistMinMax);
		terrain->SetWeightIndex(weightIndex);
		terrain->SetBrushIndex(brushIndex);

		End();
	}
}

void CTerrainGUI::SetBrushTexture(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = wstring(name.begin(), name.end());
	CTerrain* terrain = GetTarget()->Terrain();

	CTexture* tex = CResourceManager::GetInst()->FindRes<CTexture>(key).Get();
	assert(tex);

	terrain->SetBrushArrTex(tex);
}

void CTerrainGUI::SetTileTexture(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = wstring(name.begin(), name.end());
	CTerrain* terrain = GetTarget()->Terrain();

	CTexture* tex = CResourceManager::GetInst()->FindRes<CTexture>(key).Get();
	assert(tex);

	terrain->SetTileArrTex(tex);
}
