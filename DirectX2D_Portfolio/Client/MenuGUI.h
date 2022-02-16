#pragma once
#include "GUI.h"
class MenuGUI :
    public GUI
{
private :
    bool m_bPlay;
    bool m_bPause;
    bool m_bStop;

public :
    virtual void init();
    virtual void update();

private :
    void ActiveCheck();
    void CreateEmptyMaterial();
    void CreateEmptyObject();

public :
    MenuGUI();
    ~MenuGUI();
};

