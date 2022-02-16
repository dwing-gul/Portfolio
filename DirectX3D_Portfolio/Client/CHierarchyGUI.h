#pragma once
#include "CGUI.h"
#include "CTreeGUI.h"

class CHierarchyGUI :
    public CGUI
{
private :
    CTreeGUI m_tree;

public :
    virtual void init();
    virtual void update();

private :
    void RenewTree();
    void AddGameObject(CGameObject* _obj, CTreeNode* _parent);

public :
    void SelectGameObject(CTreeNode* _node);
    void DragDrop(CTreeNode* _dragStart, CTreeNode* _dragTarget);

public :
    CHierarchyGUI();
    ~CHierarchyGUI();
};

