#pragma once
#include "GUI.h"
#include "TreeGUI.h"

class HierarchyGUI :
    public GUI
{
private:
    TreeGUI m_tree;

public:
    virtual void init();
    virtual void update();

private:
    void RenewTree();
    void AddGameObject(GameObject* _obj, TreeNode* _parent);

public:
    void SelectGameObject(TreeNode* _node);
    void DragDrop(TreeNode* _dragStart, TreeNode* _dragTarget);

public:
    HierarchyGUI();
    ~HierarchyGUI();
};

