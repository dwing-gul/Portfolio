#pragma once
#include "CGUI.h"

class CTreeGUI;
class CTreeNode;

typedef void (CGUI::* SELECT_CHANGE)(CTreeNode* _node);
typedef void (CGUI::* DRAG_DROP)(CTreeNode* _draggedItem, CTreeNode* _droppedItem);

enum class TREE_TYPE
{
    HIERARCHY,
    RESOURCE
};

class CTreeNode
{
private :
    CTreeGUI* m_owner;
    CTreeNode* m_parentNode;
    vector<CTreeNode*> m_vecChildNode;
    string m_name;
    DWORD_PTR m_data;

public :
    void SetName(const string& _name) { m_name = _name; }
    void SetData(DWORD_PTR _data) { m_data = _data; }
    const string& GetName() { return m_name; }
    DWORD_PTR GetData() { return m_data; }
    
    void AddChild(CTreeNode* _child)
    {
        _child->m_parentNode = this;
        m_vecChildNode.push_back(_child); 
    }
    const vector<CTreeNode*>& GetChildNode() { return m_vecChildNode; }
    vector<CTreeNode*>& GetChildNode_Ref() { return m_vecChildNode; }

    void update();

public :
    CTreeNode();
    ~CTreeNode();

    friend class CTreeGUI;
};

class CTreeGUI :
    public CGUI
{
private :
    CTreeNode* m_root;
    CTreeNode* m_selectedNode;
    TREE_TYPE m_type;

    bool m_bRootRender;
    bool m_bFrameUse;
    bool m_bFrameOnlyParent;

    SELECT_CHANGE m_selectFunc;
    CGUI* m_selectInst;
    DRAG_DROP m_dragDropFunc;
    CGUI* m_dragDropInst;

public :
    virtual void init();
    virtual void update();

    void SetTreeType(TREE_TYPE _type) { m_type = _type; }
    void RootRender(bool _bRender) { m_bRootRender = _bRender; }
    void FrameRender(bool _bRender) { m_bFrameUse = _bRender; }
    void FrameOnlyParent(bool _choose) { m_bFrameOnlyParent = _choose; }

    bool IsRootRender() { return m_bRootRender; }
    bool IsFrameRender() { return m_bFrameUse; }
    bool IsFrameOnlyParent() { return m_bFrameOnlyParent; }

    TREE_TYPE GetTreeType() { return m_type; }
    CTreeNode* GetRootNode() { return m_root; }
    CTreeNode* AddItem(const string& _name, DWORD_PTR _data, CTreeNode* _parent);
    void SubItem(CTreeNode* _node);
    void Clear();

    void SetSelectCallBack(CGUI* _inst, SELECT_CHANGE _func) { m_selectInst = _inst; m_selectFunc = _func; }
    void SetDragDropCallBack(CGUI* _inst, DRAG_DROP _func) { m_dragDropInst = _inst; m_dragDropFunc = _func; }

private :
    void SetSelectNode(CTreeNode* _node) { m_selectedNode = _node; }
    void ItemMenu(CTreeNode* _node);

public :
    CTreeGUI();
    ~CTreeGUI();
    friend class CTreeNode;
};

