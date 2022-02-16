#pragma once
#include "State.h"

class State_Sweep :
    public State
{
private:
    Obj* m_target;
    bool m_bReady; // ���� ���� �� �ֺ� ����
    bool m_bInit; // �����̱� �� ���� ��ġ�� �޾ƿ��� ���� ����
    bool m_bSoundPlay;
    int m_dir; 
    float m_distance; // ������Ʈ�� ������ �Ÿ�
    float m_calDistance; // ������Ʈ�� ���� ������ �Ÿ�
    float m_speed; // ������Ʈ�� ���ǵ�
    Vector2 m_prevPos; // ������Ʈ�� ���� ��ġ
    Vector2 m_startPos; // ���� ��� ���� ��ġ
    Vector2 m_targetPos; // ��ǥ ��ġ
    static int m_count;
    static bool g_bLeftHandFinish;
    static bool g_bRightHandFinish;

    bool m_bLeft; // �޼��� ���������ϴ°�
    bool m_bRight; // �������� ���������ϴ°�

public:
    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_Sweep();
    ~State_Sweep();
};

