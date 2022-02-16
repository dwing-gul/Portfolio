#include "pch.h"
#include "CTileMapGUI.h"
#include "CListGUI.h"
#include "CImGuiManager.h"
#include <Engine/CTileMap.h>
#include <Engine/CResourceManager.h>

CTileMapGUI::CTileMapGUI() :
	CComponentGUI(COMPONENT_TYPE::TILEMAP)
{
}

CTileMapGUI::~CTileMapGUI()
{
}

void CTileMapGUI::update()
{
	if (Start())
	{
		CTileMap* tileMap = GetTarget()->TileMap();

		Ptr<CTexture> atlas = tileMap->GetTileAtlas();
		POINT tileMapSize = tileMap->GetTileMapSize();
		vector<tTileInfo>& vecTileInfo = tileMap->GetTileInfo();
		int size[2] = { tileMapSize.x, tileMapSize.y };

		char textureName[255] = {};
		GetResourceName(atlas.Get(), textureName);

		ImGui::Text("AtlasTexture");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##AtlasTexture", textureName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##AtlasTexture"))
		{
			CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("AtlasTexture");
			vector<wstring> vecName;
			CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vecName);
			listGUI->SetList(vecName);
			listGUI->SetDoubleClickedCallback(this, (GUI_CALLBACK)&CTileMapGUI::SetAtlas);
			listGUI->Activate();
		}

		ImGui::Text("TileMap Size");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt2("##TileMapSize", size);
		tileMapSize = POINT{ size[0], size[1] };

		ImGui::Text("Tile Infomations");
		for (size_t i = 0; i < vecTileInfo.size(); ++i)
		{
			string name = "TileInfo";
			char buffer[255] = {};
			sprintf_s(buffer, "%d", (int)i);
			name += buffer;

			if (ImGui::TreeNodeEx(name.c_str()))
			{
				ImGui::Text("LeftTop");
				ImGui::SameLine(100.f, 5.f);
				ImGui::SetNextItemWidth(180);
				name = "##TileLeftTop";
				name += buffer;
				ImGui::InputFloat2(name.c_str(), vecTileInfo[i].leftTopUV);

				ImGui::Text("TileSize");
				ImGui::SameLine(100.f, 5.f);
				ImGui::SetNextItemWidth(180);
				name = "##TileSize";
				name += buffer;
				ImGui::InputFloat2(name.c_str(), vecTileInfo[i].tileSizeUV);

				ImGui::TreePop();
			}
		}

		tileMap->SetTileAtlas(atlas);
		tileMap->SetTileMapSize(tileMapSize);

		End();
	}
}

void CTileMapGUI::SetAtlas(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = StringToWString(name);
	Ptr<CTexture> texture = CResourceManager::GetInst()->FindRes<CTexture>(key);

	GetTarget()->TileMap()->SetTileAtlas(texture);
}
