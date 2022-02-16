#include "pch.h"
#include "CTreeGUI.h"
#include "CImGuiManager.h"
#include "CInspectorGUI.h"
#include <Engine/CScript.h>
#include <Engine/CResourceManager.h>
#include <Engine\CRenderManager.h>
#include <Engine/CPrefab.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

CTreeNode::CTreeNode() :
	m_data(0),
	m_owner(nullptr),
	m_parentNode(nullptr)
{
}

CTreeNode::~CTreeNode()
{
	Safe_Delete_Vec(m_vecChildNode);
}

void CTreeNode::update()
{
	bool hasChild = true;
	if (m_vecChildNode.empty())
		hasChild = false;

	UINT flag = ImGuiTreeNodeFlags_None;

	if (m_owner->IsFrameRender() &&
		(!m_owner->IsFrameOnlyParent() ||
		(m_owner->IsFrameRender() && !m_parentNode) ||
		(!m_owner->IsRootRender() && m_parentNode == m_owner->GetRootNode())))
	{
		flag |= ImGuiTreeNodeFlags_Framed;
	}

	if(!hasChild)
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (this == m_owner->m_selectedNode)
		flag |= ImGuiTreeNodeFlags_Selected;

	string name = m_name;
	if (name.empty())
		name = "NO_NAME";

	char buffer[255] = {};
	sprintf_s(buffer, "##%ld", (int)m_data);
	name += buffer;

	if (ImGui::TreeNodeEx(name.c_str(), flag))
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
		{
			m_owner->SetSelectNode(this);
			if (m_owner->m_selectFunc && m_owner->m_selectInst)
			{
				((m_owner->m_selectInst)->*(m_owner->m_selectFunc))(m_owner->m_selectedNode);
			}
		}

		if (m_owner->GetTreeType() == TREE_TYPE::HIERARCHY)
		{
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("DraggedNode", this, sizeof(CTreeNode));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DraggedNode");

				if (payload)
				{
					CTreeNode* dragStartNode = (CTreeNode*)payload->Data;
					((m_owner->m_dragDropInst)->*(m_owner->m_dragDropFunc))(dragStartNode, this);
				}
			}


			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup("Object Item Menu");
			}
			if (ImGui::BeginPopup("Object Item Menu"))
			{
				m_owner->ItemMenu(this);
				ImGui::EndPopup();
			}
		}
		else if (TREE_TYPE::RESOURCE == m_owner->GetTreeType())
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup("Resource Menu");
			}
			if (ImGui::BeginPopup("Resource Menu"))
			{
				m_owner->ItemMenu(this);
				ImGui::EndPopup();
			}
		}

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->update();
		}

		ImGui::TreePop();
	}
}

CTreeGUI::CTreeGUI() :
	m_root(nullptr),
	m_selectedNode(nullptr),
	m_type(TREE_TYPE::HIERARCHY),
	m_bRootRender(false),
	m_bFrameUse(false),
	m_bFrameOnlyParent(false),
	m_selectFunc(nullptr),
	m_dragDropFunc(nullptr),
	m_selectInst(nullptr),
	m_dragDropInst(nullptr)
{
}

CTreeGUI::~CTreeGUI()
{
	if (nullptr != m_root)
		delete m_root;
}

void CTreeGUI::init()
{
}

void CTreeGUI::update()
{
	if (nullptr == m_root)
		return;

	if (m_bRootRender)
	{
		m_root->update();
	}
	else
	{
		const vector<CTreeNode*>& vectorChild = m_root->GetChildNode();

		for (size_t i = 0; i < vectorChild.size(); ++i)
		{
			vectorChild[i]->update();
		}
	}
}

CTreeNode* CTreeGUI::AddItem(const string& _name, DWORD_PTR _data, CTreeNode* _parent)
{
	CTreeNode* newNode = new CTreeNode;
	newNode->SetName(_name);
	newNode->SetData(_data);
	newNode->m_owner = this;

	if (nullptr == _parent)
	{
		assert(!m_root);
		m_root = newNode;
	}
	else
	{
		_parent->AddChild(newNode);
	}

	return newNode;
}

void CTreeGUI::SubItem(CTreeNode* _node)
{
	CTreeNode* parent = _node->m_parentNode;
	vector<CTreeNode*>& vectorChild = parent->GetChildNode_Ref();
	vector<CTreeNode*>::iterator iter = vectorChild.begin();

	for (; iter != vectorChild.end();)
	{
		if ((*iter) == _node)
		{
			vectorChild.erase(iter);
			delete _node;
			break;
		}
		else
		{
			++iter;
		}
	}
}

void CTreeGUI::Clear()
{
	if (nullptr != m_root)
		delete m_root;

	m_root = nullptr;
	m_selectedNode = nullptr;
}

void CTreeGUI::ItemMenu(CTreeNode* _node)
{
	if (TREE_TYPE::HIERARCHY == GetTreeType())
	{
		if (ImGui::MenuItem("Delete Object##ItemMenu"))
		{
			CInspectorGUI* inspector = (CInspectorGUI*)CImGuiManager::GetInst()->FindGUI("Inspector");
			inspector->ResetTargetObject();

			CGameObject* object = (CGameObject*)_node->GetData();
			CScript::DeleteObject(object);
			SubItem(_node);
		}

		if (ImGui::MenuItem("Prefab##ItemMenu"))
		{
			CGameObject* object = (CGameObject*)_node->GetData();
			object->RegisterAsPrefab();

			CPrefab* prefab = CResourceManager::GetInst()->FindRes<CPrefab>(object->GetName()).Get();
			wstring relativePath = L"prefab\\";
			relativePath += object->GetName();
			relativePath += L".prefab";
			prefab->Save(relativePath.c_str());
		}

		if (ImGui::MenuItem("GoToObj##ItemMenu"))
		{
			CGameObject* object = (CGameObject*)_node->GetData();

			CCamera* toolCam = CRenderManager::GetInst()->GetToolCam();

			Vec3 objPos = toolCam->Transform()->GetLocalPos();
			Vec3 objRot = toolCam->Transform()->GetLocalRot();

			toolCam->SetPrevPos(objPos);
			toolCam->SetPrevRot(objRot);

			objPos = object->Transform()->GetLocalPos();
			objRot = object->Transform()->GetLocalRot();
			objPos.z -= 200.f;

			toolCam->Transform()->SetLocalPos(objPos);
			toolCam->Transform()->SetLocalRot(objRot);
		}
	}
	else if (TREE_TYPE::RESOURCE == GetTreeType())
	{
		if (_node->m_parentNode->GetName() == "Prefab")
		{
			if (ImGui::MenuItem("Create Object##ItemMenu"))
			{
				Vector3 pos = ((CPrefab*)_node->GetData())->GetPrefabObject()->Transform()->GetLocalPos();

				CScript::Instantiate((CPrefab*)_node->GetData(), pos);
			}
		}
	}
}
