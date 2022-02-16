#pragma once
#include <Engine/Script.h>
#include <Engine/ResourceManager.h>
#include "ScriptManager.h"

enum class SKILL_TYPE
{
    SMITE,
    ZEALOUS_ACCUSATION,
    HOLY_LANCE,
    STUNNING_BLOW,
    WICKED_SLICE,
    PISTOL_SHOT,
    GRAPESHOT_BLAST,
    OPEN_VEIN,
    PLAGUE_GRENADE,
    INCISION,
    BATTLEFIELD_MEDICINE,
    DISORIENTING_BLAST,
    MACE_BASH,
    DAZZLING_LIGHT,
    DIVINE_GRACE,
    DIVINE_COMFORT,
    END
};

enum class DEBUFF_TYPE_PLAYER
{
    STUN,
    DEBUFF,
    BLEED,
    MOVE,
    BLIGHT,
    DISEASE,
    DEATHBLOW
};

class GameObject;

class PlayerScript :
    public Script
{
public :
    static GameObject*          m_selectPlayer;

private :
    GameObject*                 m_prevObj; // 플레이어 파티에서 이전에 누가 있는가?
    GameObject*                 m_nextObj; // 플레이어 파티에서 다음에 누가 있는가?
    GameObject*                 m_target;
    GameObject*                 m_damageText;
    bool                        m_bSelect;
    SKILL_TYPE                  m_skillType;
    UINT                        m_position;
    float                       m_moveSpeed;
    vector<SKILL_TYPE>          m_skillSet;

    int                         m_maxHP;
    int                         m_curHP;
    int                         m_maxStress;
    int                         m_curStress;
    int                         m_dodge;
    int                         m_protection;
    int                         m_speed;
    int                         m_turnDecision;
    int                         m_accuracyModifier;
    int                         m_criticalHitChance;
    POINT                       m_damage;
    int                         m_virtueChance;
    int                         m_stunResistance;
    int                         m_debuffResistance;
    int                         m_bleedResistance;
    int                         m_moveResistance;
    int                         m_blightResistance;
    int                         m_diseaseResistance;
    int                         m_deathblowResistance;
    int                         m_trapRelease;

    bool                        m_bHit;
    bool                        m_bCritical;
    bool                        m_bDodge;
    bool                        m_bLeftCollision;
    bool                        m_bRightCollision;
    bool                        m_bBattlePhase;
    bool                        m_bTurnStart;
    bool                        m_bTurnEnd;
    bool                        m_bAnimStart;
    bool                        m_bMove; // 애니메이션을 위한 이동
    bool                        m_bFirstMove;
    bool                        m_bHeal;
    bool                        m_bEffect;
    bool                        m_bFirst;
    bool                        m_bFirstDamageText;
    bool                        m_bDamageText;
    bool                        m_bAddStress;
    bool                        m_bDebuffDamage;
    bool                        m_bHealTarget;
    float                       m_animTime;
    float                       m_moveTime;
    float                       m_stressTime;
    float                       m_debuffDamageTime;
    float                       m_height;
    float                       m_term;
    char                        m_debuff; // 스턴, 출혈을 포함한 모든 디버프 관리
    UINT                        m_stunTurnCount;
    UINT                        m_debuffTurnCount;
    UINT                        m_bleedTurnCount;
    UINT                        m_blightTurnCount;
    UINT                        m_diseaseTurnCount;
    int                         m_bleedDamage;
    int                         m_blightDamage;

public :
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetPrevObj(GameObject* _prev) { m_prevObj = _prev; }
    void SetNextObj(GameObject* _next) { m_nextObj = _next; }
    void SetPosition(UINT _pos) { m_position = _pos; }
    void SetMaxHP(UINT _hp) { m_maxHP = _hp; }
    void SetDodge(int _dodge) { m_dodge = _dodge; }
    void SetProtection(int _protection) { m_protection = _protection; }
    void SetSpeed(int _speed) { m_speed = _speed; }
    void SetTurnDecision(int _rank) { m_turnDecision = _rank; }
    void SetAccuracyModifier(int _accuracy) { m_accuracyModifier = _accuracy; }
    void SetCriticalChance(int _critical) { m_criticalHitChance = _critical; }
    void SetDamage(POINT _damage) { m_damage = _damage; }
    void SetVirtueChance(int _virtue) { m_virtueChance = _virtue; }
    void SetStunResistance(int _stun) { m_stunResistance = _stun; }
    void SetDebuffResistance(int _debuff) { m_debuffResistance = _debuff; }
    void SetBleedResistance(int _bleed) { m_bleedResistance = _bleed; }
    void SetMoveResistance(int _move) { m_moveResistance = _move; }
    void SetBlightResistance(int _blight) { m_blightResistance = _blight; }
    void SetDiseaseResistance(int _disease) { m_diseaseResistance = _disease; }
    void SetDeathblowResistance(int _deathblow) { m_deathblowResistance = _deathblow; }
    void SetTrapRelease(int _trap) { m_trapRelease = _trap; }
    void SetSelect(bool _bSelect) { m_bSelect = _bSelect; }
    void SetSkillSet(SKILL_TYPE _type) { m_skillSet.push_back(_type); }
    void SetHit(bool _hit) { m_bHit = _hit; }
    void SetHeal(bool _heal) { m_bHeal = _heal; }
    void SetCritical(bool _bCritical) { m_bCritical = _bCritical; }
    void Hit(int _damage);
    void Dodge(bool _dodge) { m_bDodge = true; }
    void Heal(int _heal) 
    {
        m_bHeal = true;
        if(m_maxHP - m_curHP < _heal)
            m_curHP += m_maxHP - m_curHP;
        else
            m_curHP += _heal; 
    }
    void AddStress(int _stress);
    void SubStress(int _stress) 
    {
        m_curStress -= _stress;  

        if (m_curStress < 0)
        {
            m_curStress = 0;
        }
    }
    void SetBattlePhase(bool _battle) { m_bBattlePhase = _battle; }
    void SetSkillType(SKILL_TYPE _type) { m_skillType = _type; }
    void SetTurnStart(bool _turn) { m_bTurnStart = _turn; }
    void SetTurnEnd(bool _turn) { m_bTurnEnd = _turn; }
    void SetAnimStart(bool _anim) { m_bAnimStart = _anim; }
    void SetDebuff(DEBUFF_TYPE_PLAYER _type, UINT _turn, int _damage = 0);
    void SetMove(bool _move) { m_bMove = _move; }
    void SetHealTarget(bool _target) { m_bHealTarget = _target; }
    
    SKILL_TYPE GetSkillType() { return m_skillType; }
    GameObject* GetPrevObj() { return m_prevObj; }
    GameObject* GetNextObj() { return m_nextObj; }
    UINT GetPosition() { return m_position; }
    int GetMaxHP() { return m_maxHP; }
    int GetCurHP() { return m_curHP; }
    int GetMaxStress() { return m_maxStress; }
    int GetCurStress() { return m_curStress; }
    int GetProtection() { return m_protection; }
    int GetDodge() { return m_dodge; }
    int GetSpeed() { return m_speed; }
    int GetTurnDecision() { return m_turnDecision; }
    int GetAccuracyModifier() { return m_accuracyModifier; }
    int GetCriticalChance() { return m_criticalHitChance; }
    POINT GetDamage() { return m_damage; }
    int GetVirtueChance() { return m_virtueChance; }
    int GetStunResistance() { return m_stunResistance; }
    int GetDebuffResistance() { return m_debuffResistance; }
    int GetBleedResistance() { return m_bleedResistance; }
    int GetMoveResistance() { return m_moveResistance; }
    int GetBlightResistance() { return m_blightResistance; }
    int GetDiseaseResistance() { return m_diseaseResistance; }
    int GetDeathblowResistance() { return m_deathblowResistance; }
    int GetTrapRelease() { return m_trapRelease; }
    bool IsHit() { return m_bHit; }
    bool IsSelected() { return m_bSelect; }
    bool IsBattle() { return m_bBattlePhase; }
    bool IsTurnStart() { return m_bTurnStart; }
    bool IsTurnEnd() { return m_bTurnEnd; }
    bool IsStun();
    bool IsMove() { return m_bMove; }
    bool IsAnimStart() { return m_bAnimStart; }
    bool IsHeal() { return m_bHeal; }
    bool IsHealTarget() { return m_bHealTarget; }

    void SetTarget(GameObject* _target) { m_target = _target; }
    GameObject* GetTarget() { return m_target; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    virtual void SkillTrigger(GameObject* _target) = 0;

    void ReleaseDebuffByType(UINT _type);

private :
    void ReleaseDebuff();
    void CalDebuffDamage();
    void SkillSetSetting(GameObject* _skillButton);
    void InputDamageTextUI(int _damage);
    void SetDamageTextUI();

public :
    PlayerScript(SCRIPT_TYPE _type);
    ~PlayerScript();
};
