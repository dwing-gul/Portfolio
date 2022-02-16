#include "pch.h"
#include "CHierarchyGUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CEventManager.h>
#include <Engine/CSceneManager.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CScript.h>
#include "CImGuiManager.h"
#include "CInspectorGUI.h"

CHierarchyGUI::CHierarchyGUI()
{
	SetName("Hierarchy");
}

CHierarchyGUI::~CHierarchyGUI()
{
}

void CHierarchyGUI::init()
{
	m_tree.FrameRender(false);
	m_tree.RootRender(false);
	m_tree.SetSelectCallBack(this, (SELECT_CHANGE)&CHierarchyGUI::SelectGameObject);
	m_tree.SetDragDropCallBack(this, (DRAG_DROP)&CHierarchyGUI::DragDrop);
	RenewTree();
}

void CHierarchyGUI::update()
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

	if (CEventManager::GetInst()->IsHappened())
	{
		RenewTree();
	}

	if (ImGui::Begin(GetName().c_str(), &IsCancelButton()))
	{
		m_tree.update();

		ImGui::End();
	}
}

void CHierarchyGUI::RenewTree()
{
	m_tree.Clear();

	CTreeNode* root = m_tree.AddItem("ObjectRoot", 0, nullptr);

	CScene* curScene = CSceneManager::GetInst()->GetCurScene();

	if (nullptr == curScene)
		return;

	for (UINT i = 0; i < (UINT)MAX_LAYER; ++i)
	{
		CLayer* layer = curScene->GetLayer(i);

		const vector<CGameObject*>& vectorParent = layer->GetParentObj();
		for (size_t i = 0; i < vectorParent.size(); ++i)
		{
			AddGameObject(vectorParent[i], root);
		}
	}
}

void CHierarchyGUI::AddGameObject(CGameObject* _obj, CTreeNode* _parent)
{
	wstring objName = _obj->GetName();
	string name = string(objName.begin(), objName.end());
	CTreeNode* node = m_tree.AddItem(name.c_str(), (DWORD_PTR)_obj, _parent);

	const vector<CGameObject*>& vectorChild = _obj->GetChilds();
	for (size_t i = 0; i < vectorChild.size(); ++i)
	{
		AddGameObject(vectorChild[i], node);
	}
}

void CHierarchyGUI::SelectGameObject(CTreeNode* _node)
{
	CGameObject* target = (CGameObject*)_node->GetData();
	CInspectorGUI* inspector = (CInspectorGUI*)CImGuiManager::GetInst()->FindGUI("Inspector");

	inspector->SetTargetObject(target);
}

void CHierarchyGUI::DragDrop(CTreeNode* _dragStart, CTreeNode* _dragTarget)
{
	CGameObject* parent = (CGameObject*)_dragTarget->GetData();
	CGameObject* child = (CGameObject*)_dragStart->GetData();
	CScript::AddChild(parent, child);
}
