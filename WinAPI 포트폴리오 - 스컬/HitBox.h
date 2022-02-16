#pragma once
#include "Obj.h"
class HitBox :
    public Obj
{
private :
    Obj* m_owner;
    int m_damage;
    Vector2 m_offset;
    Vector2 m_effectOffset;

public :
    void SetDamage(int _damage) { m_damage = _damage; }

    virtual void update();
    virtual void render(HDC _dc);

    void SetOffset(Vector2 _offset) { m_offset = _offset; }
    void SetEffectOffset(Vector2 _offset) { m_effectOffset = _offset; }

    virtual void OnColliderEnter(Obj* _other);
    virtual void OnCollider(Obj* _other);

    CLONE(HitBox);

public :
    HitBox();
    ~HitBox();

    friend class Player;
    friend class Monster;
};

