#pragma once
#include "ComponentGUI.h"

class Light2DGUI :
    public ComponentGUI
{
private :
    Vector3 m_axis;
    bool    m_bFirst;

public :
    virtual void update();

public :
    Light2DGUI();
    ~Light2DGUI();
};

