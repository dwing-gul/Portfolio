#pragma once
#include <Engine/CGameObject.h>

class CGameObjectEX :
    public CGameObject
{
public :
    virtual void finalUpdate();

public :
    CGameObjectEX();
    ~CGameObjectEX();
};

