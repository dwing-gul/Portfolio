#pragma once
#include "State.h"

class State_Stamp :
    public State
{
private:
    Obj* m_target;
    bool m_bReady; // 동작 이행 전 주비 과정
    bool m_bInit; // 움직이기 전 최초 위치를 받아오기 위한 변수
    bool m_bSound;
    bool m_bRecoverySound;
    bool m_bGround;
    float m_distance; // 오브젝트가 움직일 거리
    float m_calDistance; // 오브젝트가 실제 움직인 거리
    float m_speed; // 오브젝트의 스피드
    static int m_count; // 내려찍기 횟수
    Vector2 m_stampDir; // 내려찍을 주먹의 방향
    Vector2 m_prevPos; // 오브젝트의 최초 위치
    Vector2 m_startPos; // 찍기 모션 시작 위치

    static bool g_bFinish;
    static bool g_bLeftHandFinish;
    static bool g_bRightHandFinish;

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_Stamp();
    ~State_Stamp();
};

