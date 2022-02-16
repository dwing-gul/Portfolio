#pragma once
#include "CPlayerScript.h"

class CAya :
    public CPlayerScript
{
private:
    bool            m_bPassiveOn;
    float           m_passiveCT;
    float           m_passiveDuration;
    
    UINT            m_clipCount;
    UINT            m_clipMaxCount;

    Vec3            m_dashEndPos;
    Vec3            m_dashDir;
    float           m_dashDistance;
    float           m_curDashDistance;
    float           m_dashSpeed;
    Vec3            m_shotDir;

    float           m_wSkillDamage;
    UINT            m_shotCount;
    bool            m_bShot;
    bool            m_bWallCol;
    UINT            m_qShotCount;

    float           m_qSkillDamage;
    float           m_Q_ReloadingTime;
    float           m_W_ReloadingTime;
    float           m_W_Distance;
    float           m_rSkillDistance;
    float           m_rSkillDuration;

    CPrefab*        m_bullet;
    CGameObject*    m_skillR_Line;

    Ptr<CSound>     m_skillQ_Attack01_Sound;
    Ptr<CSound>     m_skillQ_Attack02_Sound;
    Ptr<CSound>     m_skillW_Shot_Sound;
    Ptr<CSound>     m_skillE_Sound;
    Ptr<CSound>     m_skillR_Activation_Sound;

protected:
    virtual void LvUp();
    virtual void StatsReSet();

    virtual void Skill_Q(CGameObject* pObj);
    virtual void Skill_W(CGameObject* pObj = nullptr);
    virtual void Skill_E(CGameObject* pObj = nullptr);
    virtual void Skill_R(CGameObject* pObj = nullptr);

    void CreateBullet(CGameObject* _target = nullptr);

public:
    virtual void Attack();
    virtual void Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner = nullptr);

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void OnColliderEnter(CCollider3D* _other);
    virtual void OnCollider(CCollider3D* _other);
    virtual void OnColliderExit(CCollider3D* _other);
    virtual void SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop, float _animSpeed = 1.f, UINT _count = 0);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CAya)

public:
    CAya();
    CAya(const CAya& _origin);
    ~CAya();
};

