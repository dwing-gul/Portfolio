#pragma once
#include "State.h"

class State_Sweep :
    public State
{
private:
    Obj* m_target;
    bool m_bReady; // 동작 이행 전 주비 과정
    bool m_bInit; // 움직이기 전 최초 위치를 받아오기 위한 변수
    bool m_bSoundPlay;
    int m_dir; 
    float m_distance; // 오브젝트가 움직일 거리
    float m_calDistance; // 오브젝트가 실제 움직인 거리
    float m_speed; // 오브젝트의 스피드
    Vector2 m_prevPos; // 오브젝트의 최초 위치
    Vector2 m_startPos; // 쓸기 모션 시작 위치
    Vector2 m_targetPos; // 목표 위치
    static int m_count;
    static bool g_bLeftHandFinish;
    static bool g_bRightHandFinish;

    bool m_bLeft; // 왼손이 움직여야하는가
    bool m_bRight; // 오른손이 움직여야하는가

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_Sweep();
    ~State_Sweep();
};

