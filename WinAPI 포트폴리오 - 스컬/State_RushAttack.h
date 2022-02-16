#pragma once
#include "State.h"

class State_RushAttack :
    public State
{
private:
    Obj* m_target;
    bool m_bDash;

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_RushAttack();
    ~State_RushAttack();
};

