#pragma once
#include "Obj.h"
class NPC :
    public Obj
{
public :
    virtual void update();
    virtual void render(HDC _dc);

    CLONE(NPC)

public :
    NPC();
    virtual ~NPC();
};

