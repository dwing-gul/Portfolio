#pragma once
#include "CGUI.h"

class CMenuGUI :
    public CGUI
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
    CMenuGUI();
    ~CMenuGUI();
};

