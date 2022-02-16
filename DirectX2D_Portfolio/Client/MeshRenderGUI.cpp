#include "pch.h"
#include "MeshRenderGUI.h"
#include "ListGUI.h"
#include "ImGuiManager.h"
#include <Engine/MeshRender.h>
#include <Engine/ResourceManager.h>

MeshRenderGUI::MeshRenderGUI() :
	ComponentGUI(COMPONENT_TYPE::MESH_RENDER)
{
}

MeshRenderGUI::~MeshRenderGUI()
{
}

void MeshRenderGUI::update()
{
	if (Start())
	{
		MeshRender* meshRender = GetTarget()->GetMeshRender();

		Ptr<Mesh> mesh = meshRender->GetMesh();
		Ptr<Material> material = meshRender->GetSharedMaterial();

		char meshName[255] = {};
		GetResourceName(mesh.Get(), meshName);

		char materialName[255] = {};
		GetResourceName(material.Get(), materialName);

		ImGui::Text("Mesh");
		ImGui::SameLine(50.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Mesh", meshName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Mesh"))
		{
			ListGUI* listGUI = (ListGUI*)ImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Mesh");
			vector<wstring> vectorName;
			ResourceManager::GetInst()->GetResName(RESOURCE_TYPE::MESH, vectorName);
			listGUI->SetList(vectorName);
			listGUI->SetDoubleClickedCallBack(this, (GUI_CALLBACK)&MeshRenderGUI::SetMesh);
			listGUI->Activate();
		}

		ImGui::Text("Material");
		ImGui::SameLine(50.f, 5.f);
		ImGui::SetNextItemWidth(180);
		ImGui::InputText("##Material", materialName, 255, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select##Material"))
		{
			ListGUI* listGUI = (ListGUI*)ImGuiManager::GetInst()->FindGUI("ListGUI");
			listGUI->SetCaption("Material");
			vector<wstring> vectorName;
			ResourceManager::GetInst()->GetResName(RESOURCE_TYPE::MATERIAL, vectorName);
			listGUI->SetList(vectorName);
			listGUI->SetDoubleClickedCallBack(this, (GUI_CALLBACK)&MeshRenderGUI::SetMaterial);
			listGUI->Activate();
		}

		End();
	}
}

void MeshRenderGUI::SetMesh(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = StringToWString(name);
	Ptr<Mesh> mesh = ResourceManager::GetInst()->FindRes<Mesh>(key);

	GetTarget()->GetMeshRender()->SetMesh(mesh);
}

void MeshRenderGUI::SetMaterial(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = StringToWString(name);
	Ptr<Material> material = ResourceManager::GetInst()->FindRes<Material>(key);

	GetTarget()->GetMeshRender()->SetMaterial(material);
}
