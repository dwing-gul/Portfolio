#pragma once
#include "GUI.h"

class TreeGUI;
class TreeNode;

typedef void (GUI::*SELECT_CHANGE)(TreeNode* _node);
typedef void (GUI::*DRAG_DROP)(TreeNode* _draggedItem, TreeNode* _dropTarget);

enum class TREE_TYPE
{
    HIERARCHY,
    RESOURCE
};

class TreeNode
{
private :
    TreeGUI*            m_owner;
    TreeNode*           m_parentNode;
    vector<TreeNode*>   m_vectorChildNode;
    string              m_name;
    DWORD_PTR           m_data;

public :
    void SetName(const string& _name) { m_name = _name; }
    void SetData(DWORD_PTR _data) { m_data = _data; }
    const string& GetName() { return m_name; }
    DWORD_PTR GetData() { return m_data; }

    void AddChild(TreeNode* _child)
    {
        _child->m_parentNode = this;
        m_vectorChildNode.push_back(_child);
    }
    const vector<TreeNode*>& GetChildNode() { return m_vectorChildNode; }
    vector<TreeNode*>& GetChildNode_Ref() { return m_vectorChildNode; }

    void update();

public :
    TreeNode();
    ~TreeNode();
    friend class TreeGUI;
};

class TreeGUI :
    public GUI
{
private:
    TreeNode*       m_root;
    TreeNode*       m_selectedNode;
    TREE_TYPE       m_type;

    bool            m_bRootRender;
    bool            m_bFrameUse;
    bool            m_bFrameOnlyParent;

    SELECT_CHANGE   m_selectFunc;
    GUI*            m_selectInst;
    DRAG_DROP       m_dragDropFunc;
    GUI*            m_dragDropInst;

public:
    virtual void init();
    virtual void update();

    void SetTreeType(TREE_TYPE _type) { m_type = _type; }
    void SetRootRender(bool _bRender) { m_bRootRender = _bRender; }
    void SetFrameRender(bool _bFrameUse) { m_bFrameUse = _bFrameUse; }
    void SetFrameOnlyParent(bool _bFrameOnlyParent) { m_bFrameOnlyParent = _bFrameOnlyParent; }

    bool IsRootRender() { return m_bRootRender; }
    bool IsFrameRender() { return m_bFrameUse; }
    bool IsFrameOnlyParent() { return m_bFrameOnlyParent; }

    TREE_TYPE GetTreeType() { return m_type; }
    TreeNode* GetRootNode() { return m_root; }
    TreeNode* AddItem(const string& _name, DWORD_PTR _data, TreeNode* _parent);
    void SubItem(TreeNode* _node);
    void Clear();

    void SetSelectCallBack(GUI* _inst, SELECT_CHANGE _func) { m_selectInst = _inst; m_selectFunc = _func; }
    void SetDragDropCallBack(GUI* _inst, DRAG_DROP _func) { m_dragDropInst = _inst; m_dragDropFunc = _func; }

private :
    void SetSelectNode(TreeNode* _node) { m_selectedNode = _node; }
    void ItemMenu(TreeNode* _node);

public :
    TreeGUI();
    ~TreeGUI();
    friend class TreeNode;
};

