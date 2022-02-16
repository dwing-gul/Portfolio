#pragma once
#include "State.h"

class State_Groggy :
    public State
{
private:
    Vector2 m_prevPos; // ������Ʈ�� ���� ��ġ
    bool m_bInit; // �����̱� �� ���� ��ġ�� �޾ƿ��� ���� ����
    bool m_bSound;
    bool m_bRecoverySound;
    static bool g_bGround; // ���� ��Ҵ��� Ȯ���ϴ� ����
    static bool g_bFinish;
    float m_speed;
    float m_timer; // ���� ���� ������ �� �׷α� ���¿��� ȸ���Ǵ� �ð�

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_Groggy();
    ~State_Groggy();
};

