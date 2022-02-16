#pragma once
#include <Engine/CCamera.h>

class CCameraEX :
    public CCamera
{
public :
    virtual void finalUpdate();

public :
    CCameraEX();
    ~CCameraEX();
};

