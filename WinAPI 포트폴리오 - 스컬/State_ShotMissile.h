#pragma once
#include "State.h"
class State_ShotMissile :
    public State
{
private:
    Obj* m_target;
    bool m_bInit;
    static bool g_bShot; // ���� ���� �� �غ� ����
    static bool g_bFinish;
    bool m_bReady;
    bool m_bShot; // �̻����� ���°�
    float m_speed;
    float m_term;
    Vector2 m_prevPos;
    int m_count;

public:
    void CreateProjectile(Vector2 _offset, Vector2 _colliderOffset, Vector2 _scale, const wstring& _relativePath, PROJECTILE_TYPE _type, int _damage, Vector2 _dir);

    virtual void update();
    virtual void lateUpdate();
    virtual void Enter();
    virtual void Exit();

public:
    State_ShotMissile();
    ~State_ShotMissile();
};

