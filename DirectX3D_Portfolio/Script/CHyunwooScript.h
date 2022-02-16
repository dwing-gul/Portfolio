#pragma once
#include "CPlayerScript.h"

class CHyunwooScript :
    public CPlayerScript
{
private:
    UINT    m_passiveStack;
    UINT    m_passiveMaxStack;

    Vec3    m_assaultStratPos;          //assault 시작위치
    Vec3    m_assaultDir;               //방향
    Vec3    m_assaultEndPos;
    float   m_assaultDistance;
    float   m_curAssaultDistance;

    float   m_assaultSpeed;
    float   m_chargeTime;
    float   m_chargeMaxTime;
    float   m_chargeRadio;
    float   m_skill_W_CumTime;           //누적 시간

    bool    m_bBuff;                    //방어력 증가 버프
    bool    m_bAssaultMotion;
    bool    m_bCharge;
    bool    m_bChargeStartMotion;

    bool    m_bAssaultStartMotion;
    bool    m_bAssaultLoopMotion;
    bool    m_bAssaultEndMotion;
    bool    m_bObjCollision;
    bool    m_bWallCollsion;
    bool    m_bPassiveStackUp;

    CGameObject* m_skill_R_RangeLine;
    CGameObject* m_skill_R_Range;
    CGameObject* m_skillQ_Effect;
    CGameObject* m_skillW_Effect;

    Ptr<CSound>     m_skillQ_Attack_Sound;
    Ptr<CSound>     m_skillQ_Hit_Sound;
    Ptr<CSound>     m_skillW_Sound;
    Ptr<CSound>     m_skillE_Hit_Sound;
    Ptr<CSound>     m_skillE_Slide_Sound;
    Ptr<CSound>     m_skillR_Charging_Sound;
    Ptr<CSound>     m_skillR_Attack_Sound;
    Ptr<CSound>     m_skillR_Hit_Sound;

protected:
    virtual void LvUp();
    virtual void StatsReSet();

    virtual void Skill_Q(CGameObject* pObj = nullptr);
    virtual void Skill_W(CGameObject* pObj = nullptr);
    virtual void Skill_E(CGameObject* pObj = nullptr);
    virtual void Skill_R(CGameObject* pObj = nullptr);

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void OnColliderEnter(CCollider3D* _other);
    virtual void OnCollider(CCollider3D* _other);
    virtual void OnColliderExit(CCollider3D* _other);

    virtual void Attack();
    void PassiveActivation();
    virtual void SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop, float _animSpeed = 1.f, UINT _count = 0);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CHyunwooScript)

public:
    CHyunwooScript();
    CHyunwooScript(const CHyunwooScript& _origin);
    ~CHyunwooScript();
};
