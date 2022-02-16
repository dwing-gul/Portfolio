#pragma once
#include "State.h"

class Obj;

class State_Hit :
    public State
{
private:
    Obj* m_target;
    int m_hitCount;

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_Hit();
    ~State_Hit();
};

