#pragma once
#include "CObjectScript.h"

enum class MONSTER_STATE
{
    MONSTER_IDLE,
    MONSTER_TRACE,
    MONSTER_ATTACK,
    MONSTER_REVERT,
    MONSTER_DEAD,
    END
};

enum class MONSTER_ANIMCLIP
{
    MONSTER_ATK01,
    MONSTER_DEATH,
    MONSTER_RUN,
    MONSTER_WAIT,
    MONSTER_ATK02,
    MONSTER_APPEAR,
    MONSTER_ENDBATTLE,
    MONSTER_BEWARE_START,
    MONSTER_BEWARE_END,
    MONSTER_BEWARE_LOOP,
    MONSTER_DANCE,
    MONSTER_WAKE,
    MONSTER_SLEEP_START,
    MONSTER_SLEEP,
    MONSTER_SKILL01,
    MONSTER_APPEARWAIT,
    MONSTER_SKILL_READY,
    MONSTER_SKILL_ASSAULT,
    END
};

class CMonsterScript :
    public CObjectScript
{
private:
    APPEAR              m_appear; // 아이템 위치 파악을 위한 변수
    ITEM                m_arrBox[5]; // 몬스터 사망시 얻을 수 있는 아이템 박스

    // 공용으로 사용 되는 변수
    Vec3                m_startPos;         //몬스터가 처음 생성 되는 위치
    Vec3                m_startDir;         //몬스터가 시작 했을 때 보는 방향
    Vec3                m_CurPos;

    Vec3                m_startPosDir;

    float               m_specialAttack;
    float               m_traceDistance;   //추격 할수 있는 거리

    MONSTER_STATE       m_monsterState;
    MONSTER_TYPE        m_monsterType;
    MONSTER_ANIMCLIP    m_monsterAnimAction;        //애니메이션 동작

    float               m_AttackCooldownTime;       //공격 쿨타임
    float               m_SKillCooldownTime;        //스킬 쿨타임
    float               m_hpRecoveryCooldownTime;   //체력 회복 쿨타임

    bool                m_bSKillExistence;          //스킬 보유 여부

    bool                m_bDeadAnim;                //죽은 애니메이션 실행 여부

    bool                m_bHpRecovery;              //회복 사용 여부
    bool                m_bSkillCooldownOn;
    bool                m_bSKillMotion;             //스킬 모션중

    bool                m_bEndBattleMotion;
    bool                m_bRevert;

    CGameObject*        m_skillEffect_01;
    CGameObject*        m_skillEffect_02;

    Ptr<CSound>         m_wakeUpSound;
    Ptr<CSound>         m_skillActiveSound;
    Ptr<CSound>         m_skillImpactSound;
    Ptr<CSound>         m_skillHitSound;

private:
    // boar만 사용하는 변수
    Vec3                m_assaultStratPos;          //assault 시작위치
    Vec3                m_assaultDir;               //방향
    Vec3                m_assaultEndPos;
    float               m_curAssaultDistance;
    float               m_assaultDistance;          //돌진 거리
    bool                m_bReadyMotion;             //준비 모션인지
    bool                m_bAssaultMotion;           //돌진 모션인지

    bool                m_bMonsterMove;
    bool                m_bMonsterRenew;

private:
    virtual void CharacterDead();
    void MonsterMove();
    void MonsterItem();

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void OnColliderEnter(CCollider3D* _other);
    virtual void OnCollider(CCollider3D* _other);
    virtual void OnColliderExit(CCollider3D* _other);

public:
    void init();
    void SetStartPos(Vec3 _pos) { m_startPos = _pos; }
    Vec3 GetStartPos() { return m_startPos; }

    void SetMonsterType(MONSTER_TYPE _type);            //몬스터 타입 정하는 함수 및 기본 옵션 세팅(기본 능력치)
    void SetMonsterLv(UINT _Lv);                        // 넣어주는 레벨에 따라 몬스터 능력치를 더해줌
    void MonsterRegen();                                // 몬스터 재생성시 설정할 값들 조정

    //몬스터 State에 따른 함수
    void IdleState();
    void AttackState();
    void TraceState();
    void RevertState();
    void DeadState();

    virtual void Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner);
    virtual void Attack();
    bool AttackFrameByType();
    void SetMonsterState(MONSTER_STATE _state) { m_monsterState = _state; }
    void Skill();

    void HpRecovery();

    void SetTargetObj(CGameObject* _obj) { m_attackTarget = _obj; }

    void SetAnimAction(MONSTER_ANIMCLIP _action, bool _bLoop, float _animSpeed = 1.f, int _count = 0);

    MONSTER_STATE GetMonsterState() { return m_monsterState; }
    MONSTER_TYPE GetMonsterType() { return m_monsterType; }
    float GetMonsterHPRatio() { return m_curHP / m_maxHP; }
    float GetMonsterSPRatio() { return m_curStamina / m_maxStamina; }

    ITEM& GetBoxItem(int _index);
    void LootEnd(int _index);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CMonsterScript)

public:
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _origin);
    ~CMonsterScript();
};

