#pragma once
#include <Engine\CScript.h>
#include <Engine/CNaviTileNode.h>
#include "define.h"
#include "CScriptManager.h"

class CObjectScript :
    public CScript
{
protected :
    static CGameObject* g_itemBox;

protected:
    CNaviTileNode*          m_curNode;
    CGameObject*            m_naviMesh;
    CGameObject*            m_trackingUI;
    
    CPrefab*                m_hitEffect;

    int                     m_level;
    int                     m_maxLevel;
    float                   m_maxHP; // 최대 체력
    float                   m_curHP; // 현재 체력

    float                   m_maxStamina; // 최대 스테미나
    float                   m_curStamina; // 현재 스테미나

    float                   m_offense; // 공격력
    float                   m_defense; // 방어력
    float                   m_defense_Buff;
    float                   m_moveSpeed; // 이동 속도
    float                   m_attackSpeed; // 공격 속도
    float                   m_healthRegen; // 체력 재생
    float                   m_staminaRegen; // 스테미나 재생
    float                   m_criticalChance; // 치명타율
    float                   m_criticalDamage; // 치명타 데미지 비율
    float                   m_FOV; // 실제 시야 범위
    float                   m_attackRange; // 공격 범위
    float                   m_rotTime; // 회전시 움직일 시간
    float                   m_rotAngle; // 회전 각도

    float                   m_stunTime; // 스턴 시간
    float                   m_slowTime; // 둔화 시간
    float                   m_silenceTime; // 침묵 시간
    float                   m_defReduceTime; // 방감 시간
    float                   m_defReduce; // 방어력 감소
    float                   m_defensePenetration; // 받는 방어 관통
    float                   m_slowRate; // 이속 감소 비율
    float                   m_nonMoveTime;

    bool                    m_bStun; // 스턴
    bool                    m_bSlow; // 둔화
    bool                    m_bSilence; //침묵
    bool                    m_bNonStop; // 저지 불가
    bool                    m_bDefReduce; // 방어 감소
    bool                    m_bNonMove; // 행동 불가

    float                   m_visionRange; // 오브젝트 업데이트를 위한 시야 범위

    bool                    m_bAttack; // 공격 상태 확인
    bool                    m_bRot;
    bool                    m_bFindPath; // 길찾기를 위한 bool 값
    bool                    m_bCharacterDead; // 게임 오브젝트의 데드와는 별개로 캐릭터가 죽은 상태인지 확인하는 변수
    bool                    m_bHit;
    bool                    m_bBuff;
    bool                    m_bMotion;

    bool                    m_bAttackMotion;            //공격 모션중인지
    bool                    m_bFristAttackMotion;
    bool                    m_bAICheck;

    CGameObject*            m_attackBox; // 스킬 사용시 생성될 충돌체를 담당할 오브젝트
    CGameObject*            m_attackTarget; // 기본 공격 시 공격 대상
    CGameObject*            m_mouse;
    list<Vec3>              m_waypoints; // astar 이용시 경로를 저장하기 위한 리스트

    Ptr<CSound>             m_hitSound;
    Ptr<CSound>             m_attackSound;
    Ptr<CSound>             m_dieSound;

    bool IsPlaySound();

private:
    virtual void CharacterDead() {};
   
protected:
    virtual void StatsReSet() {};

public:
    float GetCurHP() { return m_curHP; }
    float GetCurSP() { return m_curStamina; }
    int GetLevel() { return m_level; }

    void SetDefense_Buf(float _defense) { m_defense_Buff = _defense; }
    bool IsHit() { return m_bHit; }
    bool IsCharacterDead() { return m_bCharacterDead; }
    bool IsAttack() { return m_bAttack; }
    bool IsMotion() { return m_bMotion; }
    float GetAttackRange() { return m_attackRange; }
    float GetFOV() { return m_FOV; }

    void SetStun(bool _stun, float _time) { m_bStun = _stun; m_stunTime = _time; }
    void SetSlow(bool _slow, float _time, float _slowRate) { m_bSlow = _slow; m_slowTime = _time; m_slowRate = _slowRate; }
    void SetDefReduce(bool _reduce, float _time, float _reduceRate) { m_bDefReduce = _reduce; m_defReduceTime = _time; m_defReduce = _reduceRate; }
    void SetSilence(bool _silence, float _time) { m_bSilence = _silence; m_silenceTime = _time; }
    void SetDefPenetration(float _penetration) { m_defensePenetration = _penetration; }
    void SetNonStop(bool _nonstop) { m_bNonStop = _nonstop; }
    void SetNonMove(bool _nonmove, float _time) { m_bNonMove = _nonmove; m_nonMoveTime = _time; }

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate() {}

    virtual void Attack() {}
    virtual void Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner = nullptr);

    void SetAttackTarget(CGameObject* _target) { m_attackTarget = _target; }

    bool IsCheck() { return m_bAICheck; }
    void SetCheck(bool _check) { m_bAICheck = _check; }

    virtual void OnColliderEnter(CCollider3D* _other) {}
    virtual void OnCollider(CCollider3D* _other) {}
    virtual void OnColliderExit(CCollider3D* _other) {}

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CObjectScript)

public:
    CObjectScript(UINT _type = (UINT)SCRIPT_TYPE::OBJECTSCRIPT);
    CObjectScript(const CObjectScript& _origin);
    ~CObjectScript();   
};

