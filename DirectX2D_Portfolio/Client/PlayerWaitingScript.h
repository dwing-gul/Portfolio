#pragma once
#include <Engine/Script.h>

class PlayerWaitingScript :
    public Script
{
private :
    bool m_bInput;

public:
    virtual void update();

    CLONE(PlayerWaitingScript);

public:
    PlayerWaitingScript();
    ~PlayerWaitingScript();
};

