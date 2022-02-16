#pragma once
#include "Monster.h"
class HeavyInfantry :
    public Monster
{
private :
    bool m_bGround; // 땅에 있는지 확인하는 변수
    bool m_bRightWall;// 벽의 오른쪽에 닿았는지 확인하는 변수
    bool m_bLeftWall; // 벽의 왼쪽에 닿았는지 확인하는 변수
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

