#pragma once
#include "CComponentGUI.h"

class CLight2DGUI :
    public CComponentGUI
{
private :
    Vec3    m_dir;
    bool    m_bFirst;

public :
    virtual void update();

public :
    CLight2DGUI();
    ~CLight2DGUI();
};

