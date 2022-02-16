#include "pch.h"
#include "CResourceManagerGUI.h"
#include "CImGuiManager.h"
#include "CInspectorGUI.h"
#include <Engine/CResourceManager.h>
#include <Engine/CSceneManager.h>
#include <Engine/CScene.h>

CResourceManagerGUI::CResourceManagerGUI()
{
	SetName("Resource");
}

CResourceManagerGUI::~CResourceManagerGUI()
{
}

void CResourceManagerGUI::init()
{
	m_tree.SetTreeType(TREE_TYPE::RESOURCE);
	m_tree.RootRender(false);
	m_tree.FrameOnlyParent(true);
	m_tree.FrameRender(true);
	m_tree.SetSelectCallBack(this, (SELECT_CHANGE)&CResourceManagerGUI::SelectResource);
	RenewTree();
}

void CResourceManagerGUI::update()
{
	CScene* curScene = CSceneManager::GetInst()->GetCurScene();

	if (curScene && curScene->GetSceneType() == SCENE_TYPE::PLAY_STAGE)
	{
		SetCancelButton(false);
	}

	if (!IsCancelButton())
	{
		return;
	}

	RenewResource();

	ImGui::Begin(GetName().c_str(), &IsCancelButton());
	m_tree.update();
	ImGui::End();
}

void CResourceManagerGUI::RenewResource()
{
	if (CResourceManager::GetInst()->IsHappened())
	{
		RenewTree();
	}
}

void CResourceManagerGUI::RenewTree()
{
	m_tree.Clear();

	CTreeNode* root = m_tree.AddItem("ResourceRoot", 0, nullptr);

	for (UINT i = 0; i < (UINT)RESOURCE_TYPE::END; ++i)
	{
		const map<wstring, CRes*>& mapResource = CResourceManager::GetInst()->GetRes((RESOURCE_TYPE)i);

		CTreeNode* node = m_tree.AddItem(ResourceTypeString[i], 0, root);
		for (const auto& pair : mapResource)
		{
			string name = string(pair.first.begin(), pair.first.end());
			m_tree.AddItem(name, (DWORD_PTR)pair.second, node);
		}
	}
}

void CResourceManagerGUI::SelectResource(CTreeNode* _node)
{
	if (0 == _node->GetData())
		return;

	CInspectorGUI* inspector = (CInspectorGUI*)CImGuiManager::GetInst()->FindGUI("Inspector");
	inspector->SetTargetResource((CRes*)_node->GetData());
}
