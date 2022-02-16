#include "pch.h"
#include "ResourceManagerGUI.h"
#include "ImGuiManager.h"
#include "InspectorGUI.h"
#include <Engine/ResourceManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Scene.h>

ResourceManagerGUI::ResourceManagerGUI()
{
	SetName("Resource");
}

ResourceManagerGUI::~ResourceManagerGUI()
{
}

void ResourceManagerGUI::init()
{
	m_tree.SetTreeType(TREE_TYPE::RESOURCE);
	m_tree.SetRootRender(false);
	m_tree.SetFrameRender(true);
	m_tree.SetFrameOnlyParent(true);
	m_tree.SetSelectCallBack(this, (SELECT_CHANGE)&ResourceManagerGUI::SelectResource);
	RenewTree();
}

void ResourceManagerGUI::update()
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if (curScene && curScene->GetSceneType() == SCENE_TYPE::PLAY_STAGE)
	{
		SetCancelButton(false);
	}

	if (!IsCancelButton())
	{
		return;
	}

	RenewResource();
	
	if (ImGui::Begin(GetName().c_str(), &IsCancelButton()))
	{
		m_tree.update();
		ImGui::End();
	}
}

void ResourceManagerGUI::RenewResource()
{
	if (ResourceManager::GetInst()->IsHappened())
	{
		RenewTree();
	}
}

void ResourceManagerGUI::RenewTree()
{
	m_tree.Clear();

	TreeNode* root = m_tree.AddItem("ResourceRoot", 0, nullptr);

	for (UINT i = 0; i < (UINT)RESOURCE_TYPE::END; ++i)
	{
		const map<wstring, Res*>& mapRes = ResourceManager::GetInst()->GetRes((RESOURCE_TYPE)i);

		TreeNode* node = m_tree.AddItem(ResourceTypeString[i], 0, root);
		for (const auto pair : mapRes)
		{
			string name = WStringToString(pair.first);
			m_tree.AddItem(name, (DWORD_PTR)pair.second, node);
		}
	}
}

void ResourceManagerGUI::SelectResource(TreeNode* _node)
{
	if (0 == _node->GetData())
		return;

	InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("Inspector");
	inspector->SetTargetResource((Res*)_node->GetData());
}