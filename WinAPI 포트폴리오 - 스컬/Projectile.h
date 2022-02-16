#pragma once
#include "RigidBody.h"

class Obj;

class Projectile :
    public RigidBody
{
private :
    Obj* m_owner;
    PROJECTILE_TYPE m_projectileType;
    ATTACK_TYPE m_attackType;
    Vector2 m_finalPos;
    Vector2 m_offsetPos;
    Vector2 m_dir; // 날아갈 방향

    bool m_bFirstShot;
    bool m_bGround;
    bool m_bLeftWall;
    bool m_bRightWall;
    bool m_bHead;
    bool m_bDie;

    int m_damage;

public :
    void SetDamage(int _damage) { m_damage = _damage; }
    void SetOwner(Obj* _owner) { m_owner = _owner; }
    void SetDir(Vector2 _dir) { m_dir = _dir; }
    void SetProjectileType(PROJECTILE_TYPE _type) { m_projectileType = _type; }
    void SetOffsetPos(Vector2 _offset) { m_offsetPos = _offset; }

    Vector2 GetFinalPos() { return m_finalPos; }

    bool IsHead() { return m_bHead; }

    virtual void update();
    virtual void lateUpdate();
    virtual void render(HDC _dc);

    virtual void OnColliderEnter(Obj* _other);
    virtual void OnCollider(Obj* _other);
    virtual void OnColliderExit(Obj* _other);

    CLONE(Projectile)

public :
    Projectile();
    ~Projectile();

    friend class Player;
};

