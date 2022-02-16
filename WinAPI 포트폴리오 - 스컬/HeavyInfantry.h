#pragma once
#include "Monster.h"
class HeavyInfantry :
    public Monster
{
private :
    bool m_bGround; // ���� �ִ��� Ȯ���ϴ� ����
    bool m_bRightWall;// ���� �����ʿ� ��Ҵ��� Ȯ���ϴ� ����
    bool m_bLeftWall; // ���� ���ʿ� ��Ҵ��� Ȯ���ϴ� ����
    int m_groundCount;

public :
    virtual void update();
    virtual void lateUpdate();

    virtual void OnColliderEnter(Obj* _other);
    virtual void OnCollider(Obj* _other);
    virtual void OnColliderExit(Obj* _other);

    CLONE(HeavyInfantry)

public :
    HeavyInfantry();
    ~HeavyInfantry();
};

