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
    ITEM item; // �ʿ��� ���
    int count; // �ʿ��� ����� ����
};

class CAIPointScript;

class CPlayerScript :
    public CObjectScript
{
protected:
    CAIPointScript*     m_aiPointScript;
    vector<tStuff>      m_vecAIStuff; // ��Ʈ ���
    map<wstring, ITEM>  m_mapAIItemLoot; // ���� ������ ��Ʈ
    list<CGameObject*>  m_listAttackTarget;
    list<CGameObject*>  m_listAttackTargetLootingObj;
    int                 m_curPoint;
    PLAYER_TYPE         m_playerType;
    float               m_curExp;
    float               m_exp;
    bool                m_bFindPoint; // �÷��̾���� ���� ���� ����Ʈ���� �־����� ��

    float               m_addAttackPower; // ���ݷ� ����
    float               m_addAttackPowerPerLevel; // ������ ���ݷ� ����
    float               m_addAttackSpeed; // ���� ����
    float               m_addAttackSpeedPerLevel; // ������ ���� ����
    float               m_addCriticalStrikeChance; // ġ��Ÿ Ȯ��
    float               m_addCriticalStrikeDamage; // ġ��Ÿ ���ط�
    float               m_LifeSteal; // ����� ���(�⺻ ���ݸ� �ش�)
    float               m_Omnisyphon; // ��� ���� ����
    float               m_ArmorPenetration; // ��� ����

    float               m_addMaxSP; // �ִ� ���׹̳� ����
    float               m_addSPGen; // ���׹̳� ȸ�� �߰�(������)
    float               m_addSPGen_Percent; // ���׹̳� ȸ�� �߰�(�ۼ�Ʈ)

    float               m_addDefense; // ���� ����
    float               m_addDefensePerLevel; // ������ ���� ����
    float               m_addMaxHP; // �ִ� HP ����
    float               m_addMaxHPPerLevel; // ������ �ִ� HP ����
    float               m_addHPGen; // ����� ȸ�� �߰�(������)
    float               m_addHPGen_Percent; // ����� ȸ�� �߰�(�ۼ�Ʈ)

    float               m_addMovementSpeed; // �̵� �ӵ� ����
    float               m_OutOfCombatMovementSpeed; // �������� �̵� �ӵ� ����
    float               m_addVisionRange; // �þ� ���� ����

    float               m_addAttackRange; // ���� ���� ����
    float               m_HealingReceived; // �޴� ȸ�� ����

    float               m_foodHPGen; // ���� ȿ��
    float               m_foodSPGen; // ���� ȿ��

    float               m_foodTime; // ���� ���� �ð�
    float               m_beverageTime; // ���� ���� �ð�
    Vec3                m_pointPos; // ai�� ������ ��ġ
    float               m_resetTimer; // ai�� �������� ���߰� �����ð� ���� ��ġ�� ���� ��� ������ �������� �ٲ��ֱ� ���� ����

    bool                m_bEquipChange; // �������� ��� �ߴ��� Ȯ���ϴ� ����
    bool                m_bAIMove; // ai �̵��� ����� ����
    bool                m_bFood; // ����� �������� �����ΰ�?
    bool                m_bBeverage; // ����� �������� �����ΰ�?
    bool                m_bAICollision; // ai�� ������ ���� Ȥ�� Ÿ ĳ���Ϳ� �浹�� �����ΰ�?
    bool                m_bAIRenew; // ai ������ ����
    bool                m_bGetItem; // ������ ȹ�� ��
    bool                m_bCook; // ���� ����
    bool                m_bCrafting; // ��� ����
    bool                m_bNext; 

protected:
    CHARACTER_TYPE      m_characterType;
    ITEM                m_arrEquipItems[(UINT)EQUIP_TYPE::END];
    tBelongings         m_arrInventory[10];
    vector<ITEM>        m_vecBox;
    vector<ITEM_TYPE>   m_vecWeaponType; // ĳ���Ͱ� ������ �� �ִ� ���� ����
    wstring             m_curWeaponName;
    bool                m_bBoxEmpty;

    CStructuredBuffer*  m_RHandBoneMatrix;      //������ �� ����ȭ ����
    CMaterial*          m_EquipItemMaterial;        // ���� ������ ������Ʈ�� ���׸���
    int                 m_rightHandID;          // �� ���� ��ġ
    int                 m_leftHandID;

    CGameObject*        m_mouseRayObj;
    CGameObject*        m_weapon;
    CGameObject*        m_flyingpan;        //�Ķ�����
    CGameObject*        m_craftToolFood;    // ȭ��
    CGameObject*        m_craftToolMetal;   // ���赵
    CGameObject*        m_craftDriver;      // ����̹�
    CGameObject*        m_craftHammer;      // ��ġ
    CGameObject*        m_aiLootTarget;
    CGameObject*        m_mainCam;


    Ptr<CSound>         m_areaSound;
    Ptr<CSound>         m_craftFoodSound;
    Ptr<CSound>         m_craftToolSound;
    Ptr<CSound>         m_craftCompleteSound;        // ������ ���� ��޺� ����
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

    float               m_skill_Q_CT;       //CT ��Ÿ��
    float               m_skill_W_CT;
    float               m_skill_E_CT;
    float               m_skill_R_CT;

    float               m_skill_Q_MaxCT;       //MaxCT ��Ÿ��
    float               m_skill_W_MaxCT;
    float               m_skill_E_MaxCT;
    float               m_skill_R_MaxCT;

    float               m_skill_Q_SM;       //��ų ���¹̳�
    float               m_skill_W_SM;
    float               m_skill_E_SM;
    float               m_skill_R_SM;

    bool                m_bSkill_Q_Motion;
    bool                m_bSkill_W_Motion;
    bool                m_bSkill_E_Motion;
    bool                m_bSkill_R_Motion;
    bool                m_bSkill_Motion;

    float               m_shield;           //��ȣ��

    float               m_ExtraBaseAttackDamage; // �����
    float               m_ExtraBaseAttackDamagePerLevel; // ������ �����
    float               m_ExtraSkillDamage; // ������
    float               m_ExtraSkillDamagePerLevel; // ������ ������
    float               m_SkillAmplification; // ��ų ����
    float               m_SkillAmplificationPerLevel; // ������ ��ų ����
    float               m_CooldownReduction; // ��ٿ� ����
    float               m_CooldownReductionCap; // �ִ� ��ٿ� ����

    float               m_DefenseFromBasicAttack; // �⺻ ���� ���� ����
    float               m_SkillDamageReduction; // ��ų ���� ����
    float               m_SkillDamageReduction_Percent; // ��ų ���� ����(�ۼ�Ʈ)
    float               m_CriticalDamageReduction; // ġ��Ÿ ���� ����

    float               m_midMovementSpeed; // �߰� �̵� �ӵ�(����� ���� ���)

    float               m_finalMaxHP; // ���� �ƽ� ü��
    float               m_finalMaxSP; // ���� �ִ� ���¹̳�
    float               m_finalAttackPower; // ���� ���ݷ�
    float               m_finalDefense; // ���� ����
    float               m_finalHPGen; // ���� ü�� ��
    float               m_finalSPGen; // ���� ���׹̳� ��
    float               m_finalAttackSpeed; // ���� ���� �ӵ�
    float               m_finalMovementSpeed; // ���� �̵� �ӵ�
    float               m_finalVisionRange; // ���� ĳ���� �þ� ����(������Ʈ�� ���� �þ߹��� �ƴ�)
    float               m_finalAttackRange; // ���� ���� ��Ÿ�(�⺻ ���ݸ� �ش���)
    float               m_finalCriticalChance; // ���� ġ��Ÿ Ȯ��
    float               m_finalCriticalDamage; // ���� ġ��Ÿ ���ط�

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

    // �ϴ� ���� ���� �Լ��� ó���� ����
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

    //���� ó��
    virtual void Attack();
    virtual void Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner = nullptr);
    // ����ü�� ���ư��� �ִϸ��̼� �������϶� ���ùڽ� �����ϴ� �Լ�
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

