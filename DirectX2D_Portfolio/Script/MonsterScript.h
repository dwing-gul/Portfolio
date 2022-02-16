#pragma once
#include <Engine/Script.h>
#include <Engine/ResourceManager.h>

enum class MONSTER_TYPE
{
    PROFANE,
    HUMAN,
    WOODWORK
};

enum class DEBUFF_TYPE_MONSTER
{
    STUN,
    DEBUFF,
    BLEED,
    MOVE,
    BLIGHT,
    DISEASE
};

enum class SKILL_TYPE_MONSTER
{
    RUBBLE_OF_RUIN,
    EYE_ON_YOU,
    FULMINATE,

    REND_FOR_THE_OLD_GOD,
    STUMBLING_SCRATCH,

    STRESSFUL_INCANTATION,
    ELDRITCH_PULL,
    ELDRITCH_PUSH,

    AXEBLADE,
    DEAD_WEIGHT,
    CLUMSY_AXEBLADE,

    BUMP_IN_THE_NIGHT,
    TIC_TOC,

    QURREL,
    BAYONET_JAB,

    TEMPTING_GOBLET,
    KNIFE_IN_THE_DARK,
    END
};

class MonsterScript :
    public Script
{
public :
    static bool                 g_bOverlap;
    static bool                 g_bHitEffect;

private:
    GameObject*                 m_prevObj; // 몬스터 파티에서 이전에 누가 있는가?
    GameObject*                 m_nextObj; // 몬스터 파티에서 다음에 누가 있는가?
    GameObject*                 m_target;
    GameObject*                 m_damageText;
    bool                        m_bSelect;
    UINT                        m_position;
    SKILL_TYPE_MONSTER          m_skillType;

    int                         m_maxHP;
    int                         m_curHP;
    MONSTER_TYPE                m_type;
    int                         m_dodge;
    int                         m_protection;
    int                         m_speed;
    int                         m_turnDecision;
    int                         m_stunResistance;
    int                         m_debuffResistance;
    int                         m_bleedResistance;
    int                         m_moveResistance;
    int                         m_blightResistance;

    bool                        m_bHit;
    bool                        m_bCritical;
    bool                        m_bDodge;
    bool                        m_bTurnStart;
    bool                        m_bTurnEnd;
    bool                        m_bAnimStart;
    bool                        m_bInfoOpen;
    bool                        m_bMove;
    bool                        m_bFirstMove;
    bool                        m_bEffect;
    bool                        m_bFirstDamageText;
    bool                        m_bDamageText;
    bool                        m_bDebuffDamage;
    bool                        m_bTarget;
    float                       m_animTime;
    float                       m_moveTime;
    float                       m_debuffDamageTime;
    float                       m_height;
    char                        m_debuff; // 스턴, 출혈을 포함한 모든 디버프 관리
    UINT                        m_stunTurnCount;
    UINT                        m_debuffTurnCount;
    UINT                        m_bleedTurnCount;
    UINT                        m_blightTurnCount;
    int                         m_bleedDamage;
    int                         m_blightDamage;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetMonsterType(MONSTER_TYPE _type) { m_type = _type; }
    void SetPrevObj(GameObject* _prev) { m_prevObj = _prev; }
    void SetNextObj(GameObject* _next) { m_nextObj = _next; }
    void SetPosition(UINT _pos) { m_position = _pos; }
    void SetMaxHP(UINT _hp) { m_maxHP = _hp; }
    void SetDodge(int _dodge) { m_dodge = _dodge; }
    void SetProtection(int _protection) { m_protection = _protection; }
    void SetSpeed(int _speed) { m_speed = _speed; }
    void SetTurnDecision(int _rank) { m_turnDecision = _rank; }
    void SetStunResistance(int _stun) { m_stunResistance = _stun; }
    void SetDebuffResistance(int _debuff) { m_debuffResistance = _debuff; }
    void SetBleedResistance(int _bleed) { m_bleedResistance = _bleed; }
    void SetMoveResistance(int _move) { m_moveResistance = _move; }
    void SetBlightResistance(int _blight) { m_blightResistance = _blight; }
    void SetHit(bool _hit) { m_bHit = _hit; }
    void Hit(int _damage);
    void SetCritical(bool _bCritical) { m_bCritical = _bCritical; }
    void Dodge(bool _dodge) { m_bDodge = _dodge; }
    void Heal(int _heal) { m_curHP += _heal; }
    void SetTurnStart(bool _turn) { m_bTurnStart = _turn; }
    void SetTurnEnd(bool _turn) { m_bTurnEnd = _turn; }
    void SetAnimStart(bool _anim) { m_bAnimStart = _anim; }
    void SetDebuff(DEBUFF_TYPE_MONSTER _type, UINT _turn, int _damage = 0);
    void SetSkillType(SKILL_TYPE_MONSTER _type) { m_skillType = _type; }
    void SetMove(bool _move) { m_bMove = _move; }
    void SetBoolTarget(bool _target) { m_bTarget = _target; }

    GameObject* GetPrevObj() { return m_prevObj; }
    GameObject* GetNextObj() { return m_nextObj; }
    MONSTER_TYPE GetMonsterType() { return m_type; }
    SKILL_TYPE_MONSTER GetSkillType() { return m_skillType; }
    UINT GetPosition() { return m_position; }
    int GetMaxHP() { return m_maxHP; }
    int GetCurHP() { return m_curHP; }
    int GetProtection() { return m_protection; }
    int GetDodge() { return m_dodge; }
    int GetSpeed() { return m_speed; }
    int GetTurnDecision() { return m_turnDecision; }
    int GetStunResistance() { return m_stunResistance; }
    int GetDebuffResistance() { return m_debuffResistance; }
    int GetBleedResistance() { return m_bleedResistance; }
    int GetMoveResistance() { return m_moveResistance; }
    int GetBlightResistance() { return m_blightResistance; }
    bool IsHit() { return m_bHit; }
    bool IsSelected() { return m_bSelect; }
    bool IsTurnStart() { return m_bTurnStart; }
    bool IsTurnEnd() { return m_bTurnEnd; }
    bool IsStun();
    bool IsInfoOpen() { return m_bInfoOpen; }
    bool IsMove() { return m_bMove; }
    bool IsAnimStart() { return m_bAnimStart; }
    bool IsTarget() { return m_bTarget; }

    void SetTarget(GameObject* _target) { m_target = _target; }
    GameObject* GetTarget() { return m_target; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    virtual void SkillTrigger(GameObject* _target) = 0;
    virtual void SelectTarget(const vector<GameObject*>& _playerParty) = 0;

private:
    void ReleaseDebuff();
    void CalDebuffDamage();
    void InputDamageTextUI(int _damage);
    void SetDamageTextUI();

public:
    MonsterScript(SCRIPT_TYPE _type);
    ~MonsterScript();
};

