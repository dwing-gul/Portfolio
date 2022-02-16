#pragma once
#include "State.h"

class State_Stamp :
    public State
{
private:
    Obj* m_target;
    bool m_bReady; // ���� ���� �� �ֺ� ����
    bool m_bInit; // �����̱� �� ���� ��ġ�� �޾ƿ��� ���� ����
    bool m_bSound;
    bool m_bRecoverySound;
    bool m_bGround;
    float m_distance; // ������Ʈ�� ������ �Ÿ�
    float m_calDistance; // ������Ʈ�� ���� ������ �Ÿ�
    float m_speed; // ������Ʈ�� ���ǵ�
    static int m_count; // ������� Ƚ��
    Vector2 m_stampDir; // �������� �ָ��� ����
    Vector2 m_prevPos; // ������Ʈ�� ���� ��ġ
    Vector2 m_startPos; // ��� ��� ���� ��ġ

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

