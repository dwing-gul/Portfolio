#pragma once
#include <Engine/CScript.h>

class CToolCamScript :
    public CScript
{
private :
    float m_speed;

public :
    virtual void update();

    CLONE(CToolCamScript)

public :
    CToolCamScript();
    ~CToolCamScript();
};

