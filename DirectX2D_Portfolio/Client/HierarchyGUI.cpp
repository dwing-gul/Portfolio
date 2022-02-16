#include "pch.h"
#include "HierarchyGUI.h"
#include <Engine/GameObject.h>
#include <Engine/EventManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/Script.h>
#include "ImGuiManager.h"
#include "InspectorGUI.h"

HierarchyGUI::HierarchyGUI()
{
	SetName("Hierarchy");
}

HierarchyGUI::~HierarchyGUI()
{
}

void HierarchyGUI::init()
{
	m_tree.SetTreeType(TREE_TYPE::HIERARCHY);
	m_tree.SetRootRender(false);
	m_tree.SetFrameRender(false);
	m_tree.SetFrameOnlyParent(false);
	m_tree.SetSelectCallBack(this, (SELECT_CHANGE)&HierarchyGUI::SelectGameObject);
	m_tree.SetDragDropCallBack(this, (DRAG_DROP)&HierarchyGUI::DragDrop);
	RenewTree();
}

void HierarchyGUI::update()
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

	if (EventManager::GetInst()->IsHappend())
	{
		RenewTree();
	}

	if (ImGui::Begin(GetName().c_str(), &IsCancelButton()))
	{
		m_tree.update();

		ImGui::End();
	}
}

void HierarchyGUI::RenewTree()
{
	m_tree.Clear();

	TreeNode* root = m_tree.AddItem("ObjectRoot", 0, nullptr);

	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if (nullptr == curScene)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		Layer* layer = curScene->GetLayer(i);

		const vector<GameObject*>& vectorParent = layer->GetParentObj();
		for (size_t i = 0; i < vectorParent.size(); ++i)
		{
			AddGameObject(vectorParent[i], root);
		}
	}
}

void HierarchyGUI::AddGameObject(GameObject* _obj, TreeNode* _parent)
{
	wstring objName = _obj->GetName();
	string name = WStringToString(objName);
	TreeNode* node = m_tree.AddItem(name.c_str(), (DWORD_PTR)_obj, _parent);

	const vector<GameObject*>& vectorChild = _obj->GetChilds();
	for (size_t i = 0; i < vectorChild.size(); ++i)
	{
		AddGameObject(vectorChild[i], node);
	}
}

void HierarchyGUI::SelectGameObject(TreeNode* _node)
{
	GameObject* target = (GameObject*)_node->GetData();
	InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("Inspector");
	inspector->SetTargetObject(target);
}

void HierarchyGUI::DragDrop(TreeNode* _dragStart, TreeNode* _dragTarget)
{
	GameObject* parent = (GameObject*)_dragTarget->GetData();
	GameObject* child = (GameObject*)_dragStart->GetData();
	Script::AddChild(parent, child);
}
