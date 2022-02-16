#pragma once
#include "State.h"

class State_Groggy :
    public State
{
private:
    Vector2 m_prevPos; // 오브젝트의 최초 위치
    bool m_bInit; // 움직이기 전 최초 위치를 받아오기 위한 변수
    bool m_bSound;
    bool m_bRecoverySound;
    static bool g_bGround; // 땅에 닿았는지 확인하는 변수
    static bool g_bFinish;
    float m_speed;
    float m_timer; // 죽지 않은 상태일 때 그로기 상태에서 회복되는 시간

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_Groggy();
    ~State_Groggy();
};

