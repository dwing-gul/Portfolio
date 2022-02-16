#pragma once
#include "CGUI.h"
#include "CTreeGUI.h"

class CResourceManagerGUI :
    public CGUI
{
private :
    CTreeGUI m_tree;

public :
    virtual void init();
    virtual void update();

private :
    void RenewResource();
    void RenewTree();

public :
    void SelectResource(CTreeNode* _node);

public :
    CResourceManagerGUI();
    ~CResourceManagerGUI();
};

