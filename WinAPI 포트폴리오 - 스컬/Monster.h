#pragma once
#include "RigidBody.h"

class AI;
class HitBox;
class Effect;

class Monster :
    public RigidBody
{
private :
    int m_maxHP;
    int m_curHP;
    int m_damage;
    Vector2 m_startPos;
    float m_speed;
    float m_range;
    float m_HPRatio;
    MONSTER_GROUP m_monsterType;

    HitBox* m_hitBox;

    bool m_bDie; // 죽었는지 확인하는 변수
    bool m_bHit; // 적에게 맞았는지 확인하는 변수
    bool m_bFirst;

    AI* m_ai;

protected :
    float m_dir;

public :
    virtual void update();
    virtual void lateUpdate();
    virtual void render(HDC _dc);

    void CreateAI();
    void SetHP(int _hp) { m_maxHP = _hp; }
    void SetDamage(int _damage) { m_damage = _damage; }
    void SetStartPos(Vector2 _pos) { m_startPos = _pos; }
    void SetDir(float _dir) { m_dir = _dir; }
    void SetSpeed(float _speed) { m_speed = _speed; }
    void SetRange(float _range) { m_range = _range; }
    void SetMonsterType(MONSTER_GROUP _type) { m_monsterType = _type; }
    void SetHit(bool _hit) { m_bHit = _hit; }

    bool IsHit() { return m_bHit; }
    bool IsDie() { return m_bDie; }
    AI* GetAI() { return m_ai; }
    MONSTER_GROUP GetMonsterType() { return m_monsterType; }
    float GetDir() { return m_dir; }
    float GetHPRatio() { return m_HPRatio; }
    HitBox* GetHitBox() { return m_hitBox; }
    int GetDamage() { return m_damage; }

    void Hit(int _damage);
    void CreateHitBox(Vector2 _offset, Vector2 _scale);
    void InitHitBox() { m_hitBox = nullptr; }

    void SetAnimDir(float _dir, wstring _state, float _repeat); // 방향에 따른 애니메이션을 재생하기 위한 함수

    virtual void Save(FILE* _file);
    

    CLONE(Monster);

public :
    Monster();
    virtual ~Monster();

    friend class Boss;
};

