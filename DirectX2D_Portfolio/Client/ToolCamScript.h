#pragma once
#include <Engine/Script.h>

class ToolCamScript :
    public Script
{
private :
    float m_speed;

public :
    virtual void update();

    CLONE(ToolCamScript);

public :
    ToolCamScript();
    ~ToolCamScript();
};

