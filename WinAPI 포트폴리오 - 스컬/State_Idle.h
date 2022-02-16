#pragma once
#include "State.h"

class Obj;

class State_Idle :
    public State
{
private :
    Obj* m_target;

public :
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public :
    State_Idle();
    ~State_Idle();
};

