#include "pch.h"
#include "CMeshRenderGUI.h"
#include <Engine/CMeshRender.h>
#include <Engine/CResourceManager.h>
#include "CImGuiManager.h"
#include "CListGUI.h"
#include "CParamGUI.h"
#include "CMaterialGUI.h"

CMeshRenderGUI::CMeshRenderGUI() :
	CComponentGUI(COMPONENT_TYPE::MESH_RENDER),
	m_selectIndex(0),
	m_selectedTextureParam(SHADER_PARAM::TEXTURE_END)
{
}

CMeshRenderGUI::~CMeshRenderGUI()
{
}

void CMeshRenderGUI::update()
{
	if (Start())
	{
		CMeshRender* meshRender = GetTarget()->MeshRender();

		Ptr<CMesh> mesh = meshRender->GetMesh();
		vector<Ptr<CMaterial>> vecMaterial;
		for (int i = 0; i < (int)meshRender->GetMaterialCount(); ++i)
		{
			vecMaterial.push_back(meshRender->GetSharedMaterial(i));
		}

		char meshName[255] = {};
		GetResourceName(mesh.Get(), meshName);

		vector<string> vecMaterialName;
		char materialName[255] = {};

		for (int i = 0; i < (int)meshRender->GetMaterialCount(); ++i)
		{
			if (vecMaterial[i] != nullptr)
			{
				vecMaterialName.push_back(WStringToString(vecMaterial[i].Get()->GetKey()));
			}
			else
				vecMaterialName.push_back("");
		}

		ImGui::Text("Mesh    ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Mesh", meshName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Mesh"))
		{
			CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Mesh");
			vector<wstring> vectorName;
			CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::MESH, vectorName);
			listGUI->SetList(vectorName);
			listGUI->SetDoubleClickedCallback(this, (GUI_CALLBACK)&CMeshRenderGUI::SetMesh);
			listGUI->Activate();
		}

		for (int i = 0; i < (int)meshRender->GetMaterialCount(); ++i)
		{
			ImGui::Text("Material");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(180);

			strcpy_s(materialName, 255, vecMaterialName[i].c_str());
			string name = "##Material" + std::to_string(i);
			ImGui::InputText(name.c_str(), materialName, 255, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			string select = "Select" + name;
			if (ImGui::Button(select.c_str()))
			{
				CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
				listGUI->SetCaption(name);
				vector<wstring> vectorName;
				CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::MATERIAL, vectorName);
				listGUI->SetList(vectorName);
				m_selectIndex = i;
				listGUI->SetDoubleClickedCallback(this, (GUI_CALLBACK)&CMeshRenderGUI::SetMaterial);
				listGUI->Activate();

			}

			if (nullptr != vecMaterial[i] && vecMaterial[i]->GetShader().Get())
			{
				CShader* shader = vecMaterial[i]->GetShader().Get();

				const vector<tShaderParam>& vectorParam = shader->GetShaderParam();
				CTexture* tex = nullptr;
				for (int j = 0; j < vectorParam.size(); ++j)
				{
					if (vectorParam[j].type < SHADER_PARAM::TEXTURE_0)
						continue;

					vecMaterial[i]->GetData(vectorParam[j].type, &tex);

					ImVec2 uvMin = ImVec2(0.f, 0.f);
					ImVec2 uvMax = ImVec2(1.f, 1.f);
					ImVec4 tintColor = ImVec4(1.f, 1.f, 1.f, 1.f);
					ImVec4 borderColor = ImVec4(1.f, 1.f, 1.f, 0.5f);

					ImTextureID texID = 0;
					if (nullptr != tex)
					{
						texID = (ImTextureID)tex->GetSRV().Get();
					}
					ImGui::Image(texID, ImVec2(150.f, 150.f), uvMin, uvMax, tintColor, borderColor);
				}

				Vec4 color;
				vecMaterial[i]->GetData(SHADER_PARAM::VEC4_0, &color);
				if (Vec4{} != color)
				{
					ImGui::ColorEdit4("Diff", (float*)color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				}
			}
		}

		End();
	}
}

void CMeshRenderGUI::SetMesh(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = wstring(name.begin(), name.end());
	Ptr<CMesh> mesh = CResourceManager::GetInst()->FindRes<CMesh>(key);

	GetTarget()->MeshRender()->SetMesh(mesh);
}

void CMeshRenderGUI::SetMaterial(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = wstring(name.begin(), name.end());
	Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(key);

	GetTarget()->MeshRender()->SetMaterial(material, m_selectIndex);
}
