#pragma once
#include "State.h"

class Obj;

class State_Trace :
    public State
{
private:
    Obj* m_target;

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_Trace();
    ~State_Trace();
};

