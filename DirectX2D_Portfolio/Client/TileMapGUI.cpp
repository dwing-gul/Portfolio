#include "pch.h"
#include "TileMapGUI.h"
#include "ListGUI.h"
#include "ImGuiManager.h"
#include <Engine/TileMap.h>
#include <Engine/ResourceManager.h>

TileMapGUI::TileMapGUI() :
	ComponentGUI(COMPONENT_TYPE::TILEMAP)
{
}

TileMapGUI::~TileMapGUI()
{
}

void TileMapGUI::update()
{
	if (Start())
	{
		TileMap* tileMap = GetTarget()->GetTileMap();

		Ptr<Texture> atlas = tileMap->GetTileAtlas();
		POINT tileMapSize = tileMap->GetTileMapSize();
		vector<tTileInfo>& vectorTileInfo = tileMap->GetTileInfo();
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
			ListGUI* listGUI = (ListGUI*)ImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("AtlasTexture");
			vector<wstring> vectorName;
			ResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vectorName);
			listGUI->SetList(vectorName);
			listGUI->SetDoubleClickedCallBack(this, (GUI_CALLBACK)&TileMapGUI::SetAtlas);
			listGUI->Activate();
		}

		ImGui::Text("TileMap Size");
		ImGui::SameLine(100.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputInt2("##TileMapSize", size);
		tileMapSize = POINT{ size[0], size[1] };

		ImGui::Text("Tile Infomations");
		for (size_t i = 0; i < vectorTileInfo.size(); ++i)
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
				ImGui::InputFloat2(name.c_str(), vectorTileInfo[i].leftTopUV);

				ImGui::Text("TileSize");
				ImGui::SameLine(100.f, 5.f);
				ImGui::SetNextItemWidth(180);
				name = "##TileSize";
				name += buffer;
				ImGui::InputFloat2(name.c_str(), vectorTileInfo[i].tileSizeUV);

				ImGui::TreePop();
			}
		}

		tileMap->SetTileAtlas(atlas);
		tileMap->SetTileMapSize(tileMapSize);

		End();
	}
}

void TileMapGUI::SetAtlas(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = StringToWString(name);
	Ptr<Texture> texture = ResourceManager::GetInst()->FindRes<Texture>(key);

	GetTarget()->GetTileMap()->SetTileAtlas(texture);
}
