#pragma once
#include "GUI.h"
#include "TreeGUI.h"

class ResourceManagerGUI :
    public GUI
{
private :
    TreeGUI m_tree;

public :
    virtual void init();
    virtual void update();

private :
    void RenewResource();
    void RenewTree();

public :
    void SelectResource(TreeNode* _node);

public :
    ResourceManagerGUI();
    ~ResourceManagerGUI();
};

