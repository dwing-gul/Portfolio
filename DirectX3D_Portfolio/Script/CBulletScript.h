#pragma once
#include <Engine/CScript.h>
#include "define.h"

class CBulletScript :
    public CScript
{
private :
    CGameObject*    m_owner;
    CGameObject*    m_target;
    float           m_damage;
    float           m_critical;
    float           m_extraPercent;
    float           m_extraFlat;
    float           m_dist;
    float           m_speed;
    float           m_lifeTime;
    ATTACK_TYPE     m_AttackType;

    Vec3            m_prevTargetPos;
    Vec3            m_dir;

    bool            m_bRot;
    float           m_rotAngle;
    float           m_rotTime;

    Ptr<CSound>     m_HitSound;

public :
    virtual void start();
    virtual void update();
    virtual void render() {}
    virtual void OnColliderEnter(CCollider3D* _other);

    void SetOwner(CGameObject* _owner) { m_owner = _owner; }
    void SetTarget(CGameObject* _target) { m_target = _target; }
    void SetDamage(float _damage) { m_damage = _damage; }
    void SetCritical(float _critical) { m_critical = _critical; }
    void SetExtraPercent(float _extra) { m_extraPercent = _extra; }
    void SetExtraFlat(float _extra) { m_extraFlat = _extra; }
    void SetSpeed(float _speed) { m_speed = _speed; }
    void SetAttackType(ATTACK_TYPE _type) { m_AttackType = _type; }
    void SetDir(Vec3 _dir) { m_dir = _dir; }
    
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CBulletScript);

public :
    CBulletScript();
    CBulletScript(const CBulletScript& _origin);
    ~CBulletScript();
};

