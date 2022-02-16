#pragma once
#include "CPlayerScript.h"

class CLiDailin :
    public CPlayerScript
{
private:
    bool            m_bObjCollision;
    bool            m_bWallCol;         // 벽과 충돌시

    float           m_wSkill_Duration;
    UINT            m_qSkill_Stack;
    float           m_skillDistance;
    float           m_curSkillDistance;
    UINT            m_rSkill_Stack;

    Vec3            m_skillStartPos;
    Vec3            m_skillEndPos;
    Vec3            m_skillDir;
    float           m_skillDamage;

    CGameObject*    m_skillQ_Effect;
    CGameObject*    m_skillW_Effect;
    CGameObject*    m_skillE_Effect;
    CGameObject*    m_bottle;

    CGameObject*    m_skillR_Target;
    
    Ptr<CSound>     m_skillQ_1_Sound;
    Ptr<CSound>     m_skillQ_2_Sound;
    Ptr<CSound>     m_skillQ_3_Sound;
    Ptr<CSound>     m_skillW_Sound;
    Ptr<CSound>     m_skillE_Sound;
    Ptr<CSound>     m_skillR_Dash_Sound;
    Ptr<CSound>     m_skillR_Attack_Sound;

protected:
    virtual void LvUp();
    virtual void StatsReSet();

    void SkillDirAndPosResult(float distance);

    virtual void Skill_Q(CGameObject* pObj = nullptr);
    virtual void Skill_W(CGameObject* pObj = nullptr);
    virtual void Skill_E(CGameObject* pObj = nullptr);
    virtual void Skill_R(CGameObject* pObj = nullptr);

    void Skill_R_Attack();

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void OnColliderEnter(CCollider3D* _other);
    virtual void OnCollider(CCollider3D* _other);
    virtual void OnColliderExit(CCollider3D* _other);

    virtual void SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop , float _animSpeed = 1.f, UINT _count = 0);
    
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CLiDailin)

public:
    CLiDailin();
    CLiDailin(const CLiDailin& _origin);
    ~CLiDailin();
};