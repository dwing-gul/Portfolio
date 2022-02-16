#include "pch.h"
#include "TreeGUI.h"
#include "ImGuiManager.h"
#include "InspectorGUI.h"
#include <Engine/Script.h>
#include <Engine/ResourceManager.h>
#include <Engine/Prefab.h>
#include <Engine/GameObject.h>
#include <Engine/Transform.h>

TreeNode::TreeNode() :
    m_owner(nullptr),
    m_parentNode(nullptr),
    m_data(0)
{
}

TreeNode::~TreeNode()
{
    Safe_Delete_Vector(m_vectorChildNode);
}

void TreeNode::update()
{
    bool hasChild = true;

    if (m_vectorChildNode.empty())
        hasChild = false;

    UINT flag = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

    if (m_owner->IsFrameRender() &&
        (!m_owner->IsFrameOnlyParent() || (m_owner->IsFrameRender() && !m_parentNode) || (!m_owner->IsRootRender() && m_parentNode == m_owner->GetRootNode())))
    {
        flag |= ImGuiTreeNodeFlags_Framed;
    }

    if (!hasChild)
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
        if (ImGui::IsItemClicked())
        {
            m_owner->SetSelectNode(this);
            if (m_owner->m_selectFunc && m_owner->m_selectInst)
            {
                ((m_owner->m_selectInst)->*(m_owner->m_selectFunc))(m_owner->m_selectedNode);
            }
        }

        if (TREE_TYPE::HIERARCHY == m_owner->GetTreeType())
        {
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("DraggedNode", this, sizeof(TreeNode));
                ImGui::EndDragDropSource();
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DraggedNode");

                if (payload)
                {
                    TreeNode* dragStartNode = (TreeNode*)payload->Data;
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

        for (size_t i = 0; i < m_vectorChildNode.size(); ++i)
        {
            m_vectorChildNode[i]->update();
        }

        ImGui::TreePop();
    }
}

TreeGUI::TreeGUI() :
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

TreeGUI::~TreeGUI()
{
    Safe_Delete(m_root);
}

void TreeGUI::init()
{
}

void TreeGUI::update()
{
    if (nullptr == m_root)
        return;

    if (m_bRootRender)
    {
        m_root->update();
    }
    else
    {
        const vector<TreeNode*>& vectorChild = m_root->GetChildNode();
        for (size_t i = 0; i < vectorChild.size(); ++i)
        {
            vectorChild[i]->update();
        }
    }
}

TreeNode* TreeGUI::AddItem(const string& _name, DWORD_PTR _data, TreeNode* _parent)
{
    TreeNode* newNode = new TreeNode;
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

void TreeGUI::SubItem(TreeNode* _node)
{
    TreeNode* parent = _node->m_parentNode;
    vector<TreeNode*>& vectorChild = parent->GetChildNode_Ref();
    vector<TreeNode*>::iterator iter = vectorChild.begin();

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

void TreeGUI::Clear()
{
    Safe_Delete(m_root);
    m_selectedNode = nullptr;
}

void TreeGUI::ItemMenu(TreeNode* _node)
{
    if (TREE_TYPE::HIERARCHY == GetTreeType())
    {
        if (ImGui::MenuItem("Delete Object##ItemMenu"))
        {
            InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("Inspector");
            inspector->ResetTargetObject();

            GameObject* object = (GameObject*)_node->GetData();
            Script::DeleteObject(object);
            SubItem(_node);
        }

        if (ImGui::MenuItem("Prefab##ItemMenu"))
        {
            GameObject* object = (GameObject*)_node->GetData();
            object->RegisterAsPrefab();

            Prefab* prefab = ResourceManager::GetInst()->FindRes<Prefab>(object->GetName()).Get();
            wstring relativePath = L"prefab\\";
            relativePath += object->GetName();
            relativePath += L".prefab";
            prefab->Save(relativePath.c_str());
        }
    }
    else if (TREE_TYPE::RESOURCE == GetTreeType())
    {
        if (_node->m_parentNode->GetName() == "Prefab")
        {
            if (ImGui::MenuItem("Create Object##ItemMenu"))
            {
                Vector3 pos = ((Prefab*)_node->GetData())->GetPrefabObject()->GetTransform()->GetLocalPos();

                Script::Instantiate((Prefab*)_node->GetData(), pos);
            }
        }
    }
}
