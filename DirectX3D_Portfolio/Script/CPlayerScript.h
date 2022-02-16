#pragma once
#include "CObjectScript.h"
#include "CMonsterScript.h"
#include <Engine\CCollider3D.h>

enum class PLAYER_COMMONANIM
{
    COMMON_DEATH,
    COMMON_RUN,
    COMMON_WAIT,
    COMMON_REST_END,
    COMMON_REST_LOOP,
    COMMON_REST_START,
    COMMON_COLLECT,
    COMMON_BOXOPEN,
    COMMON_FISHING,
    COMMON_CRAFTMETAL,
    COMMON_CRAFTFOOD,
    COMMON_OPERATE,
    COMMON_ARRIVE,
    COMMON_INSTALL_TRAP,
    COMMON_COLLECT_WATER,
    COMMON_COLLECT_WOOD,
    COMMON_COLLECT_STONE,
    COMMON_ENDBATTLE,
    COMMON_DANCE,
    COMMON_RESURRECT,
    COMMON_DOWN_WAIT,
    COMMON_DOWN_RUN,
    COMMON_DOWN_DEAD,
    COMMON_DOWN_START,
    END
};

enum class PLAYER_ANIMTYPE
{
    COMMON_ANIM,
    CHARACTER_ANIM,
    END
};

enum class AYA_ANIM
{
    RUN,
    WAIT,
    ATK01,
    ATK02,
    SKILL01,
    SKILL02_RUN_BACK,
    SKILL02_RUN_FORWARD,
    SKILL02_RUN_LEFT,
    SKILL02_RUN_RIGHT,
    SKILL02_WAIT,
    SKILL02_SHOT,
    SKILL03,
    SKILL04,
    SKILL04_START,
    SKILL04_END,
    WEAPONSKILL_STAR,
    WEAPONSKILL_LOOP,
    WEAPONSKILL_SHOT,
    WEAPONSKILL_END,
    RELOAD,
    END
};

enum class HYUNWOO_ANIM
{
    RUN,
    WAIT,
    ATK01,
    ATK02,
    SKILL01,
    SKILL02,
    SKILL03_START,
    SKILL03_LOOP,
    SKILL03_END,
    SKILL04_START,
    SKILL04_END,
    SKILL04_LOOP,
    SKILL04_CC_END,
    WEAPONSKILL,
    WEAPONSKILL_2,
    END
};

enum class LIDAILIN_ANIM
{
    RUN,
    WAIT,
    ATK01,
    ATK01_P,
    ATK02,
    ATK02_P,
    SKILL01_01,
    SKILL01_02,
    SKILL01_03,
    SKILL01_01_P,
    SKILL01_02_P,
    SKILL01_03_P,
    SKILL02,
    RUN_SKILL02,
    WAIT_SKILL02,
    SKILL03,
    SKILL03_P,
    SKILL04_START,
    SKILL04_START_P,
    SKILL04_ATTACK,
    SKILL04_ATTACK_P,
    WEAPONSKILL,
    END
};

struct tStuff
{
    ITEM item; // 필요한 재료
    int count; // 필요한 재료의 수량
};

class CAIPointScript;

class CPlayerScript :
    public CObjectScript
{
protected:
    CAIPointScript*     m_aiPointScript;
    vector<tStuff>      m_vecAIStuff; // 루트 재료
    map<wstring, ITEM>  m_mapAIItemLoot; // 조합 아이템 루트
    list<CGameObject*>  m_listAttackTarget;
    list<CGameObject*>  m_listAttackTargetLootingObj;
    int                 m_curPoint;
    PLAYER_TYPE         m_playerType;
    float               m_curExp;
    float               m_exp;
    bool                m_bFindPoint; // 플레이어와의 전투 이후 포인트에서 멀어졌을 때

    float               m_addAttackPower; // 공격력 증가
    float               m_addAttackPowerPerLevel; // 레벨당 공격력 증가
    float               m_addAttackSpeed; // 공속 증가
    float               m_addAttackSpeedPerLevel; // 레벨당 공속 증가
    float               m_addCriticalStrikeChance; // 치명타 확률
    float               m_addCriticalStrikeDamage; // 치명타 피해량
    float               m_LifeSteal; // 생명력 흡수(기본 공격만 해당)
    float               m_Omnisyphon; // 모든 피해 흡혈
    float               m_ArmorPenetration; // 방어 관통

    float               m_addMaxSP; // 최대 스테미나 증가
    float               m_addSPGen; // 스테미나 회복 추가(고정값)
    float               m_addSPGen_Percent; // 스테미나 회복 추가(퍼센트)

    float               m_addDefense; // 방어력 증가
    float               m_addDefensePerLevel; // 레벨당 방어력 증가
    float               m_addMaxHP; // 최대 HP 증가
    float               m_addMaxHPPerLevel; // 레벨당 최대 HP 증가
    float               m_addHPGen; // 생명력 회복 추가(고정값)
    float               m_addHPGen_Percent; // 생명력 회복 추가(퍼센트)

    float               m_addMovementSpeed; // 이동 속도 증가
    float               m_OutOfCombatMovementSpeed; // 비전투시 이동 속도 증가
    float               m_addVisionRange; // 시야 범위 증가

    float               m_addAttackRange; // 공격 범위 증가
    float               m_HealingReceived; // 받는 회복 증가

    float               m_foodHPGen; // 음식 효과
    float               m_foodSPGen; // 음료 효과

    float               m_foodTime; // 음식 섭취 시간
    float               m_beverageTime; // 음료 섭취 시간
    Vec3                m_pointPos; // ai가 움직일 위치
    float               m_resetTimer; // ai가 움직임을 멈추고 일정시간 같은 위치에 있을 경우 강제로 목적지를 바꿔주기 위한 변수

    bool                m_bEquipChange; // 아이템을 장비 했는지 확인하는 변수
    bool                m_bAIMove; // ai 이동시 사용할 변수
    bool                m_bFood; // 사용한 아이템이 음식인가?
    bool                m_bBeverage; // 사용한 아이템이 음료인가?
    bool                m_bAICollision; // ai가 아이템 스폰 혹은 타 캐릭터와 충돌한 상태인가?
    bool                m_bAIRenew; // ai 목적지 갱신
    bool                m_bGetItem; // 아이템 획득 때
    bool                m_bCook; // 음식 제작
    bool                m_bCrafting; // 장비 제작
    bool                m_bNext; 

protected:
    CHARACTER_TYPE      m_characterType;
    ITEM                m_arrEquipItems[(UINT)EQUIP_TYPE::END];
    tBelongings         m_arrInventory[10];
    vector<ITEM>        m_vecBox;
    vector<ITEM_TYPE>   m_vecWeaponType; // 캐릭터가 착용할 수 있는 무기 종류
    wstring             m_curWeaponName;
    bool                m_bBoxEmpty;

    CStructuredBuffer*  m_RHandBoneMatrix;      //오른손 본 구조화 버퍼
    CMaterial*          m_EquipItemMaterial;        // 현재 장착된 오브젝트의 메테리얼
    int                 m_rightHandID;          // 손 본의 위치
    int                 m_leftHandID;

    CGameObject*        m_mouseRayObj;
    CGameObject*        m_weapon;
    CGameObject*        m_flyingpan;        //후라이팬
    CGameObject*        m_craftToolFood;    // 화로
    CGameObject*        m_craftToolMetal;   // 설계도
    CGameObject*        m_craftDriver;      // 드라이버
    CGameObject*        m_craftHammer;      // 망치
    CGameObject*        m_aiLootTarget;
    CGameObject*        m_mainCam;


    Ptr<CSound>         m_areaSound;
    Ptr<CSound>         m_craftFoodSound;
    Ptr<CSound>         m_craftToolSound;
    Ptr<CSound>         m_craftCompleteSound;        // 아이템 제작 등급별 사운드
    Ptr<CSound>         m_skillVoiceSound;
    Ptr<CSound>         m_weapon_Sound;

    ITEM_GRADE          m_craftGrade;

    PLAYER_ANIMTYPE     m_animType;
    UINT                m_curAnim;

    UINT                m_skill_Q_Lv;
    UINT                m_skill_W_Lv;
    UINT                m_skill_E_Lv;
    UINT                m_skill_R_Lv;
    UINT                m_passive_LV;
    UINT                m_skillPoint;

    UINT                m_skill_Q_MaxLv;
    UINT                m_skill_W_MaxLv;
    UINT                m_skill_E_MaxLv;
    UINT                m_skill_R_MaxLv;
    UINT                m_passive_MaxLV;

    float               m_skill_Q_CT;       //CT 쿨타임
    float               m_skill_W_CT;
    float               m_skill_E_CT;
    float               m_skill_R_CT;

    float               m_skill_Q_MaxCT;       //MaxCT 쿨타임
    float               m_skill_W_MaxCT;
    float               m_skill_E_MaxCT;
    float               m_skill_R_MaxCT;

    float               m_skill_Q_SM;       //스킬 스태미너
    float               m_skill_W_SM;
    float               m_skill_E_SM;
    float               m_skill_R_SM;

    bool                m_bSkill_Q_Motion;
    bool                m_bSkill_W_Motion;
    bool                m_bSkill_E_Motion;
    bool                m_bSkill_R_Motion;
    bool                m_bSkill_Motion;

    float               m_shield;           //보호막

    float               m_ExtraBaseAttackDamage; // 기공추
    float               m_ExtraBaseAttackDamagePerLevel; // 레벨당 기공추
    float               m_ExtraSkillDamage; // 스공추
    float               m_ExtraSkillDamagePerLevel; // 레벨당 스공추
    float               m_SkillAmplification; // 스킬 증폭
    float               m_SkillAmplificationPerLevel; // 레벨당 스킬 증폭
    float               m_CooldownReduction; // 쿨다운 감소
    float               m_CooldownReductionCap; // 최대 쿨다운 감소

    float               m_DefenseFromBasicAttack; // 기본 공격 피해 감소
    float               m_SkillDamageReduction; // 스킬 피해 감소
    float               m_SkillDamageReduction_Percent; // 스킬 피해 감소(퍼센트)
    float               m_CriticalDamageReduction; // 치명타 피해 감소

    float               m_midMovementSpeed; // 중간 이동 속도(디버프 없는 경우)

    float               m_finalMaxHP; // 최종 맥스 체력
    float               m_finalMaxSP; // 최종 최대 스태미나
    float               m_finalAttackPower; // 최종 공격력
    float               m_finalDefense; // 최종 방어력
    float               m_finalHPGen; // 최종 체력 젠
    float               m_finalSPGen; // 최종 스테미나 젠
    float               m_finalAttackSpeed; // 최종 공격 속도
    float               m_finalMovementSpeed; // 최종 이동 속도
    float               m_finalVisionRange; // 최종 캐릭터 시야 범위(업데이트를 위한 시야범위 아님)
    float               m_finalAttackRange; // 최종 공격 사거리(기본 공격만 해당함)
    float               m_finalCriticalChance; // 최종 치명타 확률
    float               m_finalCriticalDamage; // 최종 치명타 피해량

    bool                m_bMoveMotion;
    bool                m_bTargetAttack;
    bool                m_bReLoadingMotion;
    bool                m_bGoToAttack;
    bool                m_bMouseClick;
    bool                m_bAttackDistance;
    bool                m_bChange;
    bool                m_bInit;

private:
    bool SkillLvUpCheck(UINT skillLv, UINT skillMaxLv);
    void CooldownTimeCheck();

    void ResetStat();
    void CalStat();

    virtual void CharacterDead();
    Vec3 AIMove(CNaviTileNode* _curNode, Matrix _worldMatrix);
    void AIFoodOrBeverage();

    void PlayerItemCombination();
    void AIItemCombination();
    void Skill();
    void AISkill();
    void AILooting(CPlayerScript* _target);
    void AILooting(CMonsterScript* _target);

    void CheckInvenEmpty();

protected:
    virtual void LvUp();
    virtual void StatsReSet();

    // 일단 순수 가상 함수로 처리할 예정
    virtual void Skill_Q(CGameObject* pObj = nullptr) {}
    virtual void Skill_W(CGameObject* pObj = nullptr) {}
    virtual void Skill_E(CGameObject* pObj = nullptr) {}
    virtual void Skill_R(CGameObject* pObj = nullptr) {}

public:
    map<wstring, ITEM>& GetAIItemLoot() { return m_mapAIItemLoot; }
    vector<tStuff>& GetStuff() { return m_vecAIStuff; }
    virtual void SkillLvUp(KEY _key);
    void AiSkillLvUp();

    void SetPlayerType(PLAYER_TYPE _type) { m_playerType = _type; }
    void SetEquip(bool _bChange) { m_bEquipChange = _bChange; }
    void SetFood(bool _food, float _hpGen) { m_bFood = _food; m_foodHPGen = _hpGen / 18.f; }
    void SetBeverage(bool _beverage, float _spGen) { m_bBeverage = _beverage; m_foodSPGen = _spGen / 18.f; }
    void SetChange(bool _change) { m_bChange = _change; }

    void AddStuff(ITEM _item, int _count)
    {
        tStuff stuff = {};
        stuff.item = _item;
        stuff.count = _count;

        m_vecAIStuff.push_back(stuff);
    }
    void AddLootingTree(ITEM _item);
    void SetAILootTarget(CGameObject* _target) { m_aiLootTarget = _target; }
    void SetPointPosIndex(int _index) { m_curPoint = _index; }
    void SetPointPos(Vec3 _pos) { m_pointPos = _pos; }
    void FindMonster(CGameObject* _target);
    bool IsEmptyMonster()
    {
        if (m_listAttackTarget.empty())
            return true;
        else
            return false;
    }

    int GetPointPosIndex() { return m_curPoint; }

    bool IsFood() { return m_bFood; }
    bool IsBeverage() { return m_bBeverage; }

    bool IsEquip() { return m_bEquipChange; }
    bool IsCharacterDead() { return m_bCharacterDead; }

    void AIEquipChange(EQUIP_TYPE _type);

    void SetWeaponName(ITEM_TYPE _eType);
    void SetCharacterType(CHARACTER_TYPE _type) { m_characterType = _type; }
    CHARACTER_TYPE GetCharacterType() { return m_characterType; }
    PLAYER_TYPE GetPlayerType() { return m_playerType; }
    float GetPlayerHPRatio() { return m_curHP / m_finalMaxHP; }
    float GetPlayerSPRatio() { return m_curStamina / m_finalMaxSP; }
    float GetPlayerMaxHP() { return m_finalMaxHP; }
    float GetPlayerMaxSP() { return m_finalMaxSP; }

    float GetPlayerAttackPower() { return m_finalAttackPower; }
    float GetPlayerDefense() { return m_finalDefense; }
    float GetPlayerExtraDamage() { return m_ExtraBaseAttackDamage; }
    float GetPlayerExtraDamagePerLevel() { return m_ExtraBaseAttackDamagePerLevel; }
    float GetPlayerSkillAmp() { return m_SkillAmplification; }
    float GetPlayerSkillAmpPerLevel() { return m_SkillAmplificationPerLevel; }
    float GetPlayerExtraSkillDamage() { return m_ExtraSkillDamage; }
    float GetPlayerExtraSkillDamagePerLevel() { return m_ExtraSkillDamagePerLevel; }
    float GetPlayerSkillDamageReduction() { return m_SkillDamageReduction; }
    float GetPlayerSkillDamageReductionPercent() { return m_SkillDamageReduction_Percent; }
    float GetPlayerDamageReduction() { return m_DefenseFromBasicAttack; }
    float GetPlayerMoveSpeed() { return m_finalMovementSpeed; }
    float GetPlayerAttackSpeed() { return m_finalAttackSpeed; }

    float GetPlayerExpRatio() { return m_curExp / m_exp; }
    float GetPlayerCurExp() { return m_curExp; }
    float GetPlayerMaxExp() { return m_exp; }

    const vector<ITEM_TYPE>& GetPlayerWeaponType() { return m_vecWeaponType; }
    float GetSkillCTRatio(SKILL_TYPE _type);
    bool CanSkillLevelUp(SKILL_TYPE _type);
    int GetSkillLevel(SKILL_TYPE _type);
    int GetSkillPoint() { return m_skillPoint; }
    const ITEM& GetEquipItem(int _index) { return m_arrEquipItems[_index]; }
    void ThrowEquip(int _index) { m_arrEquipItems[_index] = ITEM::END; }
    const ITEM& GetInventoryItem(int _index) 
    {
        if (_index >= 10)
            assert(nullptr);

        return m_arrInventory[_index].ID;
    }
    void ThrowItem(int _index) { m_arrInventory[_index].ID = ITEM::END; m_arrInventory[_index].count = 0; }

    bool IsSkillUsed(SKILL_TYPE _type);

    ITEM& GetBoxItem(int _index);
    void LootEnd(int _index);

    void ChangeEquipNInventoryItem(UINT _equipIndex, UINT _InventoryIndex);
    void ReleaseEquip(UINT _equipIndex);
    void LootItem(ITEM& _item);

    //어택 처리
    virtual void Attack();
    virtual void Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner = nullptr);
    // 공격체가 날아가는 애니메이션 프레임일때 어택박스 생성하는 함수
    void TargetAttack();
    void AddEXP() { m_curExp += 300.f * m_level; }

public :
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void OnColliderEnter(CCollider3D* _other);
    virtual void OnCollider(CCollider3D* _other);
    virtual void OnColliderExit(CCollider3D* _other);

    void SetAttackRange(float _range) { m_attackRange = _range; }
    virtual void SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop, float _animSpeed = 1.f, UINT _count = 0);
    
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CPlayerScript)

public :
    CPlayerScript(UINT _type = (UINT)SCRIPT_TYPE::PLAYERSCRIPT);
    CPlayerScript(const CPlayerScript& _origin);
    ~CPlayerScript();
};

