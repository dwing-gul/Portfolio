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
    APPEAR              m_appear; // ������ ��ġ �ľ��� ���� ����
    ITEM                m_arrBox[5]; // ���� ����� ���� �� �ִ� ������ �ڽ�

    // �������� ��� �Ǵ� ����
    Vec3                m_startPos;         //���Ͱ� ó�� ���� �Ǵ� ��ġ
    Vec3                m_startDir;         //���Ͱ� ���� ���� �� ���� ����
    Vec3                m_CurPos;

    Vec3                m_startPosDir;

    float               m_specialAttack;
    float               m_traceDistance;   //�߰� �Ҽ� �ִ� �Ÿ�

    MONSTER_STATE       m_monsterState;
    MONSTER_TYPE        m_monsterType;
    MONSTER_ANIMCLIP    m_monsterAnimAction;        //�ִϸ��̼� ����

    float               m_AttackCooldownTime;       //���� ��Ÿ��
    float               m_SKillCooldownTime;        //��ų ��Ÿ��
    float               m_hpRecoveryCooldownTime;   //ü�� ȸ�� ��Ÿ��

    bool                m_bSKillExistence;          //��ų ���� ����

    bool                m_bDeadAnim;                //���� �ִϸ��̼� ���� ����

    bool                m_bHpRecovery;              //ȸ�� ��� ����
    bool                m_bSkillCooldownOn;
    bool                m_bSKillMotion;             //��ų �����

    bool                m_bEndBattleMotion;
    bool                m_bRevert;

    CGameObject*        m_skillEffect_01;
    CGameObject*        m_skillEffect_02;

    Ptr<CSound>         m_wakeUpSound;
    Ptr<CSound>         m_skillActiveSound;
    Ptr<CSound>         m_skillImpactSound;
    Ptr<CSound>         m_skillHitSound;

private:
    // boar�� ����ϴ� ����
    Vec3                m_assaultStratPos;          //assault ������ġ
    Vec3                m_assaultDir;               //����
    Vec3                m_assaultEndPos;
    float               m_curAssaultDistance;
    float               m_assaultDistance;          //���� �Ÿ�
    bool                m_bReadyMotion;             //�غ� �������
    bool                m_bAssaultMotion;           //���� �������

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

    void SetMonsterType(MONSTER_TYPE _type);            //���� Ÿ�� ���ϴ� �Լ� �� �⺻ �ɼ� ����(�⺻ �ɷ�ġ)
    void SetMonsterLv(UINT _Lv);                        // �־��ִ� ������ ���� ���� �ɷ�ġ�� ������
    void MonsterRegen();                                // ���� ������� ������ ���� ����

    //���� State�� ���� �Լ�
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

