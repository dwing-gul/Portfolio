#pragma once
#include <Engine/CScript.h>

enum class DEBUFF_TYPE
{
    SLOW = 0x01,
    STUN = 0x02,
    SILENCE = 0x04,
    NONMOVE = 0x08,
    DEF_REDUCE = 0x10,
    ALL = SLOW | STUN | SILENCE | NONMOVE | DEF_REDUCE
};

class CObjectScript;

class CHitBoxScript :
    public CScript
{
private:
    CGameObject*    m_owner;
    Ptr<CSound>     m_skillHitSound;
    float           m_damage;
    float           m_critical;
    float           m_extraPercent;
    float           m_extraFlat;
    float           m_criticalChance;
    UINT            m_debuff;
    float           m_debuffTime;
    float           m_debuffRate;
    bool            m_bRate;
    bool            m_bCharacterFirstCol;

    CGameObject*    m_fristTarget;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();
    virtual void render() {}

    virtual void OnColliderEnter(CCollider3D* _other);
    virtual void OnCollider(CCollider3D* _other);
    virtual void OnColliderExit(CCollider3D* _other);

    void SetHitSound(CSound* sound) { m_skillHitSound = sound; }
    void SetOwner(CGameObject* _owner) { m_owner = _owner; }
    void SetDamage(float _damage) { m_damage = _damage; }
    void SetCritical(float _critical) { m_critical = _critical; }
    void SetCriticalChance(float _critical) { m_criticalChance = _critical; }
    void SetExtraPercent(float _extra) { m_extraPercent = _extra; }
    void SetExtraFlat(float _extra) { m_extraFlat = _extra; }
    void SetDebuff(DEBUFF_TYPE _debuff, bool _bAdapt = true);
    void SetDebuffTime(float _time) { m_debuffTime = _time; }
    void SetDebuffRate(float _rate) { m_debuffRate = _rate; }
    void SetRate(bool _bRate) { m_bRate = _bRate; }

    void Reset();
    bool IsPlaySound();
    bool IsCharacterFirstCol() { return m_bCharacterFirstCol; }
    CGameObject* GetFristTarGetObj() { return m_fristTarget; }

private :
    void CheckDebuff(CObjectScript* _target);

    CLONE(CHitBoxScript);

public:
    CHitBoxScript();
    ~CHitBoxScript();
};

