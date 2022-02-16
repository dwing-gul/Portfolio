#include "pch.h"
#include "CPlayerScript.h"
#include "CMouseRay.h"
#include "CItemManager.h"
#include "CItemSpawnScript.h"
#include "CLayerUpdateBox.h"
#include "CIGItemObjScript.h"
#include "CHitBoxScript.h"
#include "CItemBoxUIScript.h"
#include "CEffectScript.h"
#include "CAIPointScript.h"

#include <Engine\CRenderManager.h>
#include <Engine\CSceneManager.h>
#include <Engine\CCamera.h>
#include <Engine\CTransform.h>
#include <Engine\CNaviMesh.h>
#include <Engine\CScene.h>
#include <Engine\CLayer.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CStructuredBuffer.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>

CPlayerScript::CPlayerScript(UINT _type) :
	CObjectScript(_type),
	m_aiPointScript(nullptr),
	m_curPoint(0),
	m_playerType(PLAYER_TYPE::PLAYER),
	m_skill_Q_Lv(0),
	m_skill_W_Lv(0),
	m_skill_E_Lv(0),
	m_skill_R_Lv(0),
	m_passive_LV(1),
	m_skillPoint(0),
	m_skill_Q_MaxLv(5),
	m_skill_W_MaxLv(5),
	m_skill_E_MaxLv(5),
	m_skill_R_MaxLv(3),
	m_passive_MaxLV(3),
	m_skill_Q_CT(0.f),
	m_skill_W_CT(0.f),
	m_skill_E_CT(0.f),
	m_skill_R_CT(0.f),
	m_skill_Q_MaxCT(0.f),
	m_skill_W_MaxCT(0.f),
	m_skill_E_MaxCT(0.f),
	m_skill_R_MaxCT(0.f),
	m_skill_Q_SM(0.f),
	m_skill_W_SM(0.f),
	m_skill_E_SM(0.f),
	m_skill_R_SM(0.f),
	m_shield(0.f),
	m_curExp(0.f),
	m_exp(600.f),
	m_bSkill_Q_Motion(false),
	m_bSkill_W_Motion(false),
	m_bSkill_E_Motion(false),
	m_bSkill_R_Motion(false),
	m_bBoxEmpty(true),
	m_bNext(false),
	m_addAttackPower(0.f),
	m_addAttackPowerPerLevel(0.f),
	m_addAttackSpeed(0.f),
	m_addAttackSpeedPerLevel(0.f),
	m_addCriticalStrikeChance(0.f),
	m_addCriticalStrikeDamage(0.f),
	m_LifeSteal(0.f),
	m_Omnisyphon(0.f),
	m_ExtraBaseAttackDamage(0.f),
	m_ExtraBaseAttackDamagePerLevel(0.f),
	m_ArmorPenetration(0.f),
	m_ExtraSkillDamage(0.f),
	m_ExtraSkillDamagePerLevel(0.f),
	m_SkillAmplification(0.f),
	m_SkillAmplificationPerLevel(0.f),
	m_CooldownReduction(0.f),
	m_pointPos(Vec3(0.f, 0.f, 0.f)),
	m_resetTimer(0.f),
	m_addMaxSP(0.f),
	m_addSPGen(0.f),
	m_addSPGen_Percent(0.f),
	m_addDefense(0.f),
	m_addDefensePerLevel(0.f),
	m_addMaxHP(0.f),
	m_addMaxHPPerLevel(0.f),
	m_addHPGen(0.f),
	m_addHPGen_Percent(0.f),
	m_DefenseFromBasicAttack(0.f),
	m_SkillDamageReduction(0.f),
	m_SkillDamageReduction_Percent(0.f),
	m_CriticalDamageReduction(0.f),
	m_addMovementSpeed(0.f),
	m_OutOfCombatMovementSpeed(0.f),
	m_addVisionRange(0.f),
	m_addAttackRange(0.f),
	m_midMovementSpeed(0.f),
	m_CooldownReductionCap(0.f),
	m_HealingReceived(0.f),
	m_bEquipChange(false),
	m_bAIMove(false),
	m_bFood(false),
	m_bBeverage(false),
	m_bAICollision(false),
	m_bCook(false),
	m_bCrafting(false),
	m_bAIRenew(false),
	m_bGetItem(false),
	m_bFindPoint(false),
	m_foodHPGen(0.f),
	m_foodSPGen(0.f),
	m_foodTime(18.f),
	m_beverageTime(18.f),
	m_finalAttackPower(m_offense),
	m_finalAttackRange(m_attackRange),
	m_finalAttackSpeed(m_attackSpeed),
	m_finalDefense(m_defense),
	m_finalHPGen(m_healthRegen),
	m_finalSPGen(m_staminaRegen),
	m_finalMaxHP(m_maxHP),
	m_finalMaxSP(m_maxStamina),
	m_finalMovementSpeed(m_moveSpeed),
	m_finalVisionRange(m_FOV),
	m_finalCriticalChance(m_criticalChance),
	m_finalCriticalDamage(m_criticalDamage),
	m_curAnim(0),
	m_animType(PLAYER_ANIMTYPE::END),
	m_bMoveMotion(false),
	m_characterType(CHARACTER_TYPE::END),
	m_rightHandID(0),
	m_leftHandID(0),
	m_mouseRayObj(nullptr),
	m_bSkill_Motion(false),
	m_weapon(nullptr),
	m_flyingpan(nullptr),
	m_craftToolFood(nullptr),
	m_craftToolMetal(nullptr),
	m_craftDriver(nullptr),
	m_craftHammer(nullptr),
	m_EquipItemMaterial(nullptr),
	m_aiLootTarget(nullptr),
	m_bTargetAttack(false),
	m_bReLoadingMotion(false),
	m_craftGrade(ITEM_GRADE::NONE),
	m_bGoToAttack(false),
	m_mainCam(nullptr),
	m_bMouseClick(false),
	m_bAttackDistance(false),
	m_bChange(true),
	m_bInit(true)
{
	for (UINT i = 1; i < (UINT)EQUIP_TYPE::END; ++i)
	{
		m_arrEquipItems[i] = ITEM::END;
	}

	for (int i = 0; i < 10; ++i)
	{
		m_arrInventory[i].ID = ITEM::END;
		m_arrInventory[i].count = 0;
	}

	m_RHandBoneMatrix = new CStructuredBuffer;
	m_RHandBoneMatrix->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(Matrix), 1, nullptr);
}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin) :
	CObjectScript(_origin),
	m_aiPointScript(_origin.m_aiPointScript),
	m_curPoint(0),
	m_playerType(_origin.m_playerType),
	m_skill_Q_Lv(_origin.m_skill_Q_Lv),
	m_skill_W_Lv(_origin.m_skill_W_Lv),
	m_skill_E_Lv(_origin.m_skill_E_Lv),
	m_skill_R_Lv(_origin.m_skill_R_Lv),
	m_passive_LV(_origin.m_passive_LV),
	m_skillPoint(_origin.m_skillPoint),
	m_skill_Q_MaxLv(5),
	m_skill_W_MaxLv(5),
	m_skill_E_MaxLv(5),
	m_skill_R_MaxLv(3),
	m_passive_MaxLV(3),
	m_skill_Q_CT(0.f),
	m_skill_W_CT(0.f),
	m_skill_E_CT(0.f),
	m_skill_R_CT(0.f),
	m_skill_Q_MaxCT(0.f),
	m_skill_W_MaxCT(0.f),
	m_skill_E_MaxCT(0.f),
	m_skill_R_MaxCT(0.f),
	m_skill_Q_SM(_origin.m_skill_Q_SM),
	m_skill_W_SM(_origin.m_skill_W_SM),
	m_skill_E_SM(_origin.m_skill_E_SM),
	m_skill_R_SM(_origin.m_skill_R_SM),
	m_shield(0.f),
	m_curExp(_origin.m_curExp),
	m_exp(_origin.m_exp),
	m_bSkill_Q_Motion(false),
	m_bSkill_W_Motion(false),
	m_bSkill_E_Motion(false),
	m_bSkill_R_Motion(false),
	m_bNext(false),
	m_bFindPoint(false),
	m_bBoxEmpty(true),
	m_addAttackPower(0.f),
	m_addAttackPowerPerLevel(0.f),
	m_addAttackSpeed(0.f),
	m_addAttackSpeedPerLevel(0.f),
	m_addCriticalStrikeChance(0.f),
	m_addCriticalStrikeDamage(0.f),
	m_LifeSteal(0.f),
	m_Omnisyphon(0.f),
	m_ExtraBaseAttackDamage(0.f),
	m_ExtraBaseAttackDamagePerLevel(0.f),
	m_ArmorPenetration(0.f),
	m_ExtraSkillDamage(0.f),
	m_ExtraSkillDamagePerLevel(0.f),
	m_SkillAmplification(0.f),
	m_SkillAmplificationPerLevel(0.f),
	m_CooldownReduction(0.f),
	m_pointPos(Vec3(0.f, 0.f, 0.f)),
	m_resetTimer(0.f),
	m_addMaxSP(0.f),
	m_addSPGen(0.f),
	m_addSPGen_Percent(0.f),
	m_addDefense(0.f),
	m_addDefensePerLevel(0.f),
	m_addMaxHP(0.f),
	m_addMaxHPPerLevel(0.f),
	m_addHPGen(0.f),
	m_addHPGen_Percent(0.f),
	m_DefenseFromBasicAttack(0.f),
	m_SkillDamageReduction(0.f),
	m_SkillDamageReduction_Percent(0.f),
	m_CriticalDamageReduction(0.f),
	m_addMovementSpeed(0.f),
	m_OutOfCombatMovementSpeed(0.f),
	m_addVisionRange(0.f),
	m_addAttackRange(0.f),
	m_midMovementSpeed(0.f),
	m_CooldownReductionCap(0.f),
	m_HealingReceived(0.f),
	m_bEquipChange(false),
	m_bAIMove(false),
	m_bFood(false),
	m_bBeverage(false),
	m_bAICollision(false),
	m_bCook(false),
	m_bCrafting(false),
	m_bAIRenew(false),
	m_bGetItem(false),
	m_foodHPGen(0.f),
	m_foodSPGen(0.f),
	m_foodTime(18.f),
	m_beverageTime(18.f),
	m_finalAttackPower(m_offense),
	m_finalAttackRange(m_attackRange),
	m_finalAttackSpeed(m_attackSpeed),
	m_finalDefense(m_defense),
	m_finalHPGen(m_healthRegen),
	m_finalSPGen(m_staminaRegen),
	m_finalMaxHP(m_maxHP),
	m_finalMaxSP(m_maxStamina),
	m_finalMovementSpeed(m_moveSpeed),
	m_finalVisionRange(m_FOV),
	m_finalCriticalChance(m_criticalChance),
	m_finalCriticalDamage(m_criticalDamage),
	m_curAnim(0),
	m_animType(PLAYER_ANIMTYPE::END),
	m_bMoveMotion(false),
	m_characterType(_origin.m_characterType),
	m_rightHandID(_origin.m_rightHandID),
	m_leftHandID(_origin.m_leftHandID),
	m_mouseRayObj(nullptr),
	m_bSkill_Motion(false),
	m_weapon(nullptr),
	m_flyingpan(nullptr),
	m_craftToolFood(nullptr),
	m_craftToolMetal(nullptr),
	m_craftDriver(nullptr),
	m_craftHammer(nullptr),
	m_EquipItemMaterial(nullptr),
	m_aiLootTarget(nullptr),
	m_bTargetAttack(false),
	m_bReLoadingMotion(false),
	m_craftGrade(ITEM_GRADE::NONE),
	m_bGoToAttack(false),
	m_mainCam(nullptr),
	m_bMouseClick(false),
	m_bAttackDistance(false),
	m_bChange(true),
	m_bInit(true)
{
		for (UINT i = 1; i < (UINT)EQUIP_TYPE::END; ++i)
		{
			m_arrEquipItems[i] = ITEM::END;
		}

		for (int i = 0; i < 10; ++i)
		{
			m_arrInventory[i].ID = ITEM::END;
			m_arrInventory[i].count = 0;
		}

		m_RHandBoneMatrix = new CStructuredBuffer;
		m_RHandBoneMatrix->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(Matrix), 1, nullptr);
}


CPlayerScript::~CPlayerScript()
{
	if (nullptr != m_RHandBoneMatrix)
	{
		delete m_RHandBoneMatrix;
	}

	Safe_Delete_Ptr(m_aiPointScript);
}

void CPlayerScript::start()
{
	StatsReSet();

	CObjectScript::start();

	if (nullptr == m_mainCam)
	{
		m_mainCam = CSceneManager::GetInst()->FindObjByName(L"MainCamera");
	}

	if (nullptr == m_trackingUI)
	{
		Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"PlayerTrackingUI", L"prefab\\RootTrackingUI.prefab");
		m_trackingUI = prefab->Instantiate();
		for (size_t i = 0; i < m_trackingUI->GetChilds().size(); ++i)
		{
			if(!m_trackingUI->GetChilds()[i]->GetScripts().empty())
				((CUIScript*)m_trackingUI->GetChilds()[i]->GetScripts()[0])->SetTarget(Object());
		}
		((CUIScript*)m_trackingUI->GetScripts()[0])->SetTarget(Object());
		CScript::CreateObject(m_trackingUI, Vec3(0.f, 0.f, 0.f), 31);
	}

	if (nullptr == m_aiPointScript)
	{
		Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"AiPoint", L"prefab\\AiPoint.prefab");
		CGameObject* obj = prefab->Instantiate();
		m_aiPointScript = (CAIPointScript*)obj->GetScripts()[0]->Clone();
		m_aiPointScript->SetOwner(Object());

		delete obj;
	}

	m_curPoint = m_aiPointScript->GetFindPointID();

	vector<CGameObject*> childObj = Object()->GetChilds();

	if (!childObj.empty())
	{
		CIGItemObjScript* IgItemScript;
		for (int i = 0; i < childObj.size(); ++i)
		{
			if (L"Flyingpan" == childObj[i]->GetName())
			{
				m_flyingpan = childObj[i];
				IgItemScript = (CIGItemObjScript*)m_flyingpan->GetScripts()[0];
				IgItemScript->SetOwnerObj(Object());
				IgItemScript->SetEquipOption(true, m_rightHandID);
				IgItemScript->SetLocalMatrixUpdate(true);
			}
			if (L"CraftToolFood" == childObj[i]->GetName())
			{
				m_craftToolFood = childObj[i];
			}
			if (L"CraftToolMetal" == childObj[i]->GetName())
			{
				m_craftToolMetal = childObj[i];
				((CIGItemObjScript*)m_craftToolMetal->GetScripts()[0])->SetOwnerObj(Object());
			}
			if (L"CraftDriver" == childObj[i]->GetName())
			{
				m_craftDriver = childObj[i];
				IgItemScript = (CIGItemObjScript*)m_craftDriver->GetScripts()[0];
				IgItemScript->SetOwnerObj(Object());
				IgItemScript->SetEquipOption(true, m_rightHandID);
				IgItemScript->SetLocalMatrixUpdate(true);
			}
			if (L"CraftHammer" == childObj[i]->GetName())
			{
				m_craftHammer = childObj[i];
				IgItemScript = (CIGItemObjScript*)m_craftHammer->GetScripts()[0];
				IgItemScript->SetOwnerObj(Object());
				IgItemScript->SetEquipOption(true, m_rightHandID);
				IgItemScript->SetLocalMatrixUpdate(true);
			}
		}
	}

	if (nullptr == m_flyingpan)
	{
		CPrefab* flyingpan = CResourceManager::GetInst()->Load<CPrefab>(L"Flyingpan", L"prefab\\Flyingpan.prefab").Get();
		m_flyingpan = flyingpan->Instantiate();
		m_flyingpan->SetName(L"Flyingpan");
		CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_flyingpan->GetScripts()[0];
		IgItemScript->SetOwnerObj(Object());
		IgItemScript->SetEquipOption(true, m_rightHandID);
		IgItemScript->SetLocalMatrixUpdate(true);
		CScript::AddChild(Object(), m_flyingpan);
	}

	if (nullptr == m_craftToolFood)
	{
		CPrefab* craftToolFood = CResourceManager::GetInst()->Load<CPrefab>(L"CraftToolFood", L"prefab\\CraftToolFood.prefab").Get();
		m_craftToolFood = craftToolFood->Instantiate();
		m_craftToolFood->SetName(L"CraftToolFood");
		CScript::AddChild(Object(), m_craftToolFood);
	}

	if (nullptr == m_craftToolMetal)
	{
		CPrefab* craftToolMetal = CResourceManager::GetInst()->Load<CPrefab>(L"CraftToolMetal", L"prefab\\CraftToolMetal.prefab").Get();
		m_craftToolMetal = craftToolMetal->Instantiate();
		m_craftToolMetal->SetName(L"CraftToolMetal");
		((CIGItemObjScript*)m_craftToolMetal->GetScripts()[0])->SetOwnerObj(Object());
		CScript::AddChild(Object(), m_craftToolMetal);
	}

	if (nullptr == m_craftDriver)
	{
		CPrefab* CraftDriver = CResourceManager::GetInst()->Load<CPrefab>(L"CraftDriver", L"prefab\\CraftDriver.prefab").Get();
		m_craftDriver = CraftDriver->Instantiate();
		m_craftDriver->SetName(L"CraftDriver");
		CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_craftDriver->GetScripts()[0];
		IgItemScript->SetOwnerObj(Object());
		IgItemScript->SetEquipOption(true, m_rightHandID);
		IgItemScript->SetLocalMatrixUpdate(true);
		CScript::AddChild(Object(), m_craftDriver);
	}

	if (nullptr == m_craftHammer)
	{
		CPrefab* craftHammer = CResourceManager::GetInst()->Load<CPrefab>(L"CraftHammer", L"prefab\\CraftHammer.prefab").Get();
		m_craftHammer = craftHammer->Instantiate();
		m_craftHammer->SetName(L"CraftHammer");
		CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_craftHammer->GetScripts()[0];
		IgItemScript->SetOwnerObj(Object());
		IgItemScript->SetEquipOption(true, m_rightHandID);
		IgItemScript->SetLocalMatrixUpdate(true);
		CScript::AddChild(Object(), m_craftHammer);
	}

	m_flyingpan->SetObjOff();
	m_craftToolFood->SetObjOff();
	m_craftToolMetal->SetObjOff();
	m_craftDriver->SetObjOff();
	m_craftHammer->SetObjOff();

	m_mouseRayObj = CSceneManager::GetInst()->FindObjByName(L"Mouse");

	// 사운드 로드
	// 경로 폴더이름 변경후 다시
	m_craftFoodSound = CResourceManager::GetInst()->Load<CSound>(L"character_Craft_Food", L"sound\\character\\chracter_Craft_Food.wav");
	m_craftToolSound = CResourceManager::GetInst()->Load<CSound>(L"character_Craft_Tool", L"sound\\character\\chracter_Craft_Tool.wav");

	switch (m_characterType)
	{
	case CHARACTER_TYPE::AYA:
	{
		CResourceManager::GetInst()->Load<CSound>(L"Aya_craftUncommon", L"sound\\aya\\Aya_craftUncommon_1_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"Aya_craftRare", L"sound\\aya\\Aya_craftRare_2_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"Aya_craftEpic", L"sound\\aya\\Aya_craftEpic_1_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"Aya_craftLegend", L"sound\\aya\\Aya_craftLegend_1_ko.wav");
	}
		break;
	case CHARACTER_TYPE::HYUNWOO:
	{
		CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_craftUncommon", L"sound\\hyunwoo\\Hyunwoo_craftUncommon_1_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_craftRare", L"sound\\hyunwoo\\Hyunwoo_craftRare_2_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_craftEpic", L"sound\\hyunwoo\\Hyunwoo_craftEpic_1_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_craftLegend", L"sound\\hyunwoo\\Hyunwoo_craftLegend_1_ko.wav");
	}
		break;
	case CHARACTER_TYPE::LIDAILIN:
	{
		CResourceManager::GetInst()->Load<CSound>(L"LiDailin_craftUncommon", L"sound\\LiDailin\\LiDailin_craftUncommon_1_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"LiDailin_craftRare", L"sound\\LiDailin\\LiDailin_craftRare_2_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"LiDailin_craftEpic", L"sound\\LiDailin\\LiDailin_craftEpic_1_ko.wav");
		CResourceManager::GetInst()->Load<CSound>(L"LiDailin_craftLegend", L"sound\\LiDailin\\LiDailin_craftLegend_1_ko.wav");
	}
		break;
	case CHARACTER_TYPE::END:
		break;
	default:
		break;
	}
}

void CPlayerScript::update()
{
	if (m_bCharacterDead)
		return;

	if (Object()->GetName() == L"Player" && KEY_TAP(KEY::L))
	{
		m_playerType = PLAYER_TYPE::PLAYER == m_playerType ? PLAYER_TYPE::AI : PLAYER_TYPE::PLAYER;
	}

	// 다른 캐릭터와의 전투 중 캐릭터 간의 거리가 공격 사거리 + 100 보다 멀어졌거나 죽였을 경우
	if (PLAYER_TYPE::AI == m_playerType && nullptr != m_attackTarget && m_attackTarget->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		Vec3 pos = Transform()->GetLocalPos();
		Vec3 targetPos = m_attackTarget->Transform()->GetLocalPos();

		if (((CObjectScript*)m_attackTarget->GetScripts()[0])->IsCharacterDead() || Vec3::Distance(pos, targetPos) > m_attackRange + 100.f)
		{
			m_bFindPoint = true;
			m_attackTarget = nullptr;
			m_curPoint = m_aiPointScript->GetFindPointID();
			m_bAIMove = false;
			m_bFindPath = true;
			return;
		}
	}

	CObjectScript::update();

	if(SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode())
		m_curExp += (1000.f / 60.f) * DELTATIME;

	if (PLAYER_TYPE::AI == m_playerType)
	{
		// 현재 next가 true이기 때문에 바로 addpointid를 하게 되어서 문제가 생기는 것임.
		// 이를 해결하는 가장 간단한 방법은 지점별로 next와 arrive가 있으면 될것으로 추정
		// 공통의 next와 arrive가 있는 경우에도 해결할 수 있는지는 고민을 해봐야할 듯.
		if (m_aiPointScript->GetPointType(m_curPoint) == POINT_TYPE::LOOTING && nullptr == m_aiLootTarget && m_aiPointScript->IsArrive())
		{
			m_aiPointScript->SetNext(true);
		}
		else if (m_aiPointScript->GetPointType(m_curPoint) == POINT_TYPE::HUNTING && 
			((m_listAttackTargetLootingObj.empty() && m_listAttackTarget.empty()) || m_curHP / m_finalMaxHP < 0.3f) && m_aiPointScript->IsArrive())
		{
			m_aiPointScript->SetNext(true);

			if (m_curHP / m_finalMaxHP < 0.3f)
			{
				m_listAttackTarget.clear();
				m_listAttackTargetLootingObj.clear();
			}
		}

		int prevID = m_aiPointScript->GetPrevPointID();

		if (m_aiPointScript->IsNext())
		{
			m_aiPointScript->SetArrive(false);

			if (m_aiPointScript->GetPointType(m_curPoint) != POINT_TYPE::MOVE && m_bNext)
			{
				m_bNext = false;
				m_aiPointScript->AddPointID();
			}
		}

		m_curPoint = m_aiPointScript->GetCurPointID();

		// 플레이어에서 ai로 전환되었을 때 찾아가는 경우
		if (m_bChange)
		{
			m_bChange = false;
			m_aiPointScript->SetArrive(false);
			m_aiPointScript->SetNext(false);
			m_bFindPoint = true;
			m_curPoint = m_aiPointScript->GetFindPointID();
			m_bAIMove = false;
			m_waypoints.clear();
		}
	}

	if (m_curHP > m_finalMaxHP)
	{
		m_curHP = m_finalMaxHP;
	}

	if (m_curStamina > m_finalMaxSP)
	{
		m_curStamina = m_finalMaxSP;
	}

	if (!m_bSkill_Motion && m_attackBox != nullptr && m_attackBox->IsOn())
	{
		m_attackBox->SetOnOff(false);
		((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuff(DEBUFF_TYPE::ALL, false);
		((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetRate(false);
	}

	if (m_bSlow)
	{
		m_finalMovementSpeed = m_midMovementSpeed * (1 - m_slowRate);
	}
	else
	{
		m_finalMovementSpeed = m_midMovementSpeed;
	}

	if (m_bEquipChange)
	{
		m_bEquipChange = false;
	}
	CalStat();

	if (m_bInit)
	{
		m_bInit = false;
		m_curHP = m_finalMaxHP;
		m_curStamina = m_finalMaxSP;
	}

	if (m_curHP < m_finalMaxHP)
	{
		m_curHP += m_finalHPGen * DELTATIME;

		if (m_curHP > m_finalMaxHP)
		{
			m_curHP = m_finalMaxHP;
		}
	}

	if (m_curStamina < m_finalMaxSP)
	{
		m_curStamina += m_finalSPGen * DELTATIME;

		if (m_curStamina > m_finalMaxSP)
		{
			m_curStamina = m_finalMaxSP;
		}
	}

	if (PLAYER_TYPE::PLAYER == m_playerType)
	{
		if (KEY_HOLD(KEY::LCTRL) && m_skillPoint > 0 &&
			(KEY_TAP(KEY::Q) || KEY_TAP(KEY::W) || KEY_TAP(KEY::E) || KEY_TAP(KEY::R) || KEY_TAP(KEY::T)))
		{
			//플레이어 레벨마다 스킬 최대 레벨 조정해주기 
			if (KEY_TAP(KEY::Q))
				SkillLvUp(KEY::Q);
			else if (KEY_TAP(KEY::W))
				SkillLvUp(KEY::W);
			else if (KEY_TAP(KEY::E))
				SkillLvUp(KEY::E);
			else if (KEY_TAP(KEY::R))
				SkillLvUp(KEY::R);
			else if (KEY_TAP(KEY::T))
				SkillLvUp(KEY::T);
		}
	}
	else
	{
		if (0 < m_skillPoint)
			AiSkillLvUp();
	}

	if (PLAYER_TYPE::PLAYER == m_playerType)
	{
		if (KEY_TAP(KEY::G) && !m_bCrafting && !m_bCook && !m_bMotion && !m_bAttack)
		{
			// 아이템 조합
			PlayerItemCombination();
		}

		if ((m_bCrafting || m_bCook) && Animator3D()->IsAnimFinish())
		{
			PlayerItemCombination();
		}
	}
	else
	{
		CheckInvenEmpty();

		if (m_bGetItem && !m_bAttack && !m_bSkill_Motion)
		{
			AIItemCombination();
		}
	}

	if (m_bCrafting || m_bCook)
	{
		if (m_bCook)
		{
			if (m_flyingpan->IsOn() && Animator3D()->GetCurFrameIndex() == 240)
			{
				m_flyingpan->SetObjOff();
				if (nullptr != m_weapon)
				{
					m_weapon->SetObjOn();
					CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_weapon->GetScripts()[0];
					m_EquipItemMaterial = IgItemScript->GetCurMaterial();
					Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
				}
			}
		}
		if (m_bCrafting)
		{
			if (Animator3D()->GetCurFrameIndex() == 10 && nullptr != m_craftHammer && !m_craftHammer->IsOn())
			{
				m_craftHammer->SetObjOn();
				CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_craftHammer->GetScripts()[0];
				m_EquipItemMaterial = IgItemScript->GetCurMaterial();
				Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
			}
			else if (Animator3D()->GetCurFrameIndex() == 90 && nullptr != m_craftHammer && m_craftHammer->IsOn())
			{
				m_craftHammer->SetObjOff();
				if (nullptr != m_craftDriver && !m_craftDriver->IsOn())
				{
					CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_craftDriver->GetScripts()[0];
					m_EquipItemMaterial = IgItemScript->GetCurMaterial();
					Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
				}
			}
			else if (Animator3D()->GetCurFrameIndex() == 190 && nullptr != m_craftDriver && m_craftDriver->IsOn())
			{
				m_craftDriver->SetObjOff();
				Animator3D()->SetBoneMaterial(nullptr);
			}
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		if (m_arrInventory[i].count <= 0)
		{
			m_arrInventory[i].ID = ITEM::END;
		}
	}

	if (m_curExp >= m_exp)
	{
		m_curExp -= m_exp;
		m_exp += 600.f;
		LvUp();
	}

	if (!m_bMotion)
	{
		if (PLAYER_TYPE::PLAYER == m_playerType)
		{
			Skill();
			m_bSkill_Motion = (m_bSkill_Q_Motion || m_bSkill_W_Motion || m_bSkill_E_Motion || m_bSkill_R_Motion) == true ? true : false;
		}
		else
		{
			if (!m_bAttackMotion)
			{
				AISkill();
				m_bSkill_Motion = (m_bSkill_Q_Motion || m_bSkill_W_Motion || m_bSkill_E_Motion || m_bSkill_R_Motion) == true ? true : false;
			}
		}
	}

	AIFoodOrBeverage();

	if (m_bFood)
	{
		m_foodTime -= DELTATIME;
		m_curHP += m_foodHPGen * DELTATIME;

		if (m_curHP > m_finalMaxHP)
		{
			m_curHP = m_finalMaxHP;
		}

		if (m_foodTime < 0.f)
		{
			m_bFood = false;
			m_foodTime = 18.f;
		}
	}

	if (m_bBeverage)
	{
		m_beverageTime -= DELTATIME;

		m_curStamina += m_foodSPGen * DELTATIME;

		if (m_curStamina > m_finalMaxSP)
		{
			m_curStamina = m_finalMaxSP;
		}

		if (m_beverageTime < 0.f)
		{
			m_bBeverage = false;
			m_beverageTime = 18.f;
		}
	}

	if (m_bAttack) // 기본 공격 상태 일때
	{
		if (!m_bAttackMotion && !m_bSkill_Motion)
		{
			if (!m_bFristAttackMotion)
			{
				switch (m_characterType)
				{
				case CHARACTER_TYPE::AYA:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::ATK01, false);
					break;
				case CHARACTER_TYPE::HYUNWOO:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::ATK01, false);
					break;
				case CHARACTER_TYPE::LIDAILIN:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::ATK01, false);
					break;
				case CHARACTER_TYPE::END:
					return;
				default:
					return;
				}
			}
			else
			{
				switch (m_characterType)
				{
				case CHARACTER_TYPE::AYA:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::ATK02, false);
					break;
				case CHARACTER_TYPE::HYUNWOO:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::ATK02, false);
					break;
				case CHARACTER_TYPE::LIDAILIN:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::ATK02, false);
					break;
				case CHARACTER_TYPE::END:
					return;
				default:
					return;
				}
			}

			m_bAttackMotion = true;
		}

		if (Animator3D()->IsAnimFinish())
		{
			m_bFristAttackMotion = m_bFristAttackMotion == false ? true : false;
			m_bAttack = false;
			m_bTargetAttack = false;
			m_bAttackMotion = false;

		}
	}
	else
	{
		if (!m_bMoveMotion && !m_bMotion && !m_bAttack && !m_bSkill_Motion)
		{
			if (!m_curWeaponName.empty())
			{
				switch (m_characterType)
				{
				case CHARACTER_TYPE::AYA:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, true);
					break;
				case CHARACTER_TYPE::HYUNWOO:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::WAIT, true);
					break;
				case CHARACTER_TYPE::LIDAILIN:
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::WAIT, true);
					break;
				case CHARACTER_TYPE::END:
					break;
				default:
					break;
				}
			}
			else
				SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_WAIT, true);
		}
	}

	// 현재 플레이어가 실제 플레이어 인 경우
	// A 키를 누르고 왼,우 클릭을 하게 되는 경우 마우스 피킹 테스트를 하여
	// 오브젝트를 찾는다. 만약 피킹된 오브젝트가 nullptr이 아니고 몬스터 혹은 AI인 경우
	// 해당 타겟을 공격하러 간다.
	if (PLAYER_TYPE::PLAYER == m_playerType)
	{
		if (KEY_TAP(KEY::A))
		{
			m_bGoToAttack = true;//m_bGoToAttack == true ? false : true;
			m_attackTarget = nullptr;
		}

		if (m_bGoToAttack)
		{
			if (KEY_TAP(KEY::LBUTTON) && nullptr == m_attackTarget && CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
			{
				m_attackTarget = ((CMouseRay*)m_mouseRayObj->GetScripts()[0])->MousePicking();
				m_bMouseClick = true;

				if (nullptr == m_attackTarget || (OBJECT_TYPE::PLAYER != m_attackTarget->GetObjectType()
					&& OBJECT_TYPE::MONSTER != m_attackTarget->GetObjectType()))
				{
					m_attackTarget = nullptr;
					m_bGoToAttack = false;
					m_naviMesh->NaviMesh()->SetRenew(false);
					m_bMouseClick = false;
				}
			}
		}
	}
}

void CPlayerScript::lateUpdate()
{
	if (m_bCharacterDead)
		return;

	if (PLAYER_TYPE::AI == m_playerType && nullptr == m_aiPointScript)
		return;

	m_bSkill_Motion = (m_bSkill_Q_Motion || m_bSkill_W_Motion || m_bSkill_E_Motion || m_bSkill_R_Motion) == true ? true : false;
	m_bMotion = (m_bSkill_Motion || m_bReLoadingMotion || m_bCook || m_bCrafting) == true ? true : false;

	if (Animator3D())
	{
		Animator3D()->SetMatrixBuffer(m_RHandBoneMatrix);
	}

	if (nullptr == m_mouseRayObj)
	{
		m_mouseRayObj = CSceneManager::GetInst()->FindObjByName(L"Mouse");
	}

	CooldownTimeCheck();

	if (nullptr == m_naviMesh)
	{
		m_naviMesh = CSceneManager::GetInst()->FindObjByName(L"NaviMesh");
	}

	Vec3 pointPos;

	if (nullptr != m_naviMesh)
	{
		bool bRenew = m_naviMesh->NaviMesh()->IsRenew();

		// 공격이 아닐 땐 이동을 하고 공격을 할 때는 애니메이션 전환
		if ((!m_bAttackMotion || bRenew) && !m_bSkill_Motion && SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode())
		{
			// 플레이어 이동 및 그에 따른 회전
			if (nullptr != m_naviMesh)
			{
				bool bMove = m_naviMesh->NaviMesh()->IsMove();

				// 캐릭터 이동 및 회전
				Vec3 dir = Vec3(0.f, 0.f, 0.f);
				Vec3 pos = Transform()->GetLocalPos();
				Vec3 rot = Transform()->GetLocalRot();
				Vec3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				front.Normalize();
				float distance = 0.f;
				Vec3 waypoint;

				if (PLAYER_TYPE::PLAYER == m_playerType)
				{
					if (nullptr == m_attackTarget)
					{
						pointPos = m_naviMesh->NaviMesh()->GetPointPos();
					}
					else
						pointPos = m_attackTarget->Transform()->GetLocalPos();

					if (m_bFindPath)
					{
						m_bFindPath = false;
						m_curNode = m_naviMesh->NaviMesh()->GetCurNode(Object());
						m_naviMesh->NaviMesh()->FindPath(m_curNode, Object(), pointPos, m_naviMesh->Transform()->GetWorldMatrix(), m_waypoints);
					}

					if (bRenew && !m_bMouseClick)
					{
						m_bGoToAttack = false;
						m_bAttack = false;
						m_bTargetAttack = false;
						m_bAttackMotion = false;
						m_attackTarget = nullptr;
					}
					else if (bRenew)
						m_bMouseClick = false;

					if (m_waypoints.size() != 0)
					{
						waypoint = m_waypoints.front();
					}

					waypoint.y = pos.y;
					pointPos.y = pos.y;

					Vec3 checkDir = Vec3(0.f, 0.f, 0.f);
					float checkDist = 0.f;

					if (bMove || bRenew)
					{
						if (!m_bMoveMotion && !m_bAttackMotion)
						{
							if (!m_curWeaponName.empty())
							{
								switch (m_characterType)
								{
								case CHARACTER_TYPE::AYA:
									SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::RUN, true);
									break;
								case CHARACTER_TYPE::HYUNWOO:
									SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::RUN, true);
									break;
								case CHARACTER_TYPE::LIDAILIN:
									SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::RUN, true);
									break;
								case CHARACTER_TYPE::END:
									break;
								default:
									break;
								}
							}
							else
								SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_RUN, true);

							if (m_bCook)
							{
								m_bCook = false;

								if (nullptr != m_craftToolFood && m_craftToolFood->IsOn())
									m_craftToolFood->SetObjOff();
								if (nullptr != m_flyingpan && m_flyingpan->IsOn())
									m_flyingpan->SetObjOff();

								if (nullptr != m_craftFoodSound)
								{
									m_craftFoodSound->Stop();
								}

								m_bMotion = false;
							}

							if (m_bCrafting)
							{
								m_bCrafting = false;

								if (nullptr != m_craftToolMetal && m_craftToolMetal->IsOn())
									m_craftToolMetal->SetObjOff();
								if (nullptr != m_craftDriver && m_craftDriver->IsOn())
									m_craftDriver->SetObjOff();
								if (nullptr != m_craftHammer && m_craftHammer->IsOn())
									m_craftHammer->SetObjOff();

								if (nullptr != m_craftToolSound)
								{
									m_craftToolSound->Stop();
								}

								m_bMotion = false;
							}

							if (m_bReLoadingMotion)
							{
								m_bReLoadingMotion = false;
								m_weapon_Sound->Stop();
							}

							m_bMoveMotion = true;

							
						}

						dir.x = pointPos.x - pos.x;
						dir.z = pointPos.z - pos.z;
						distance = dir.Length();

						dir.x = waypoint.x - pos.x;
						dir.z = waypoint.z - pos.z;
						distance = dir.Length();

						checkDir.x = pointPos.x - pos.x;
						checkDir.z = pointPos.z - pos.z;
						checkDist = checkDir.Length();
						checkDir.Normalize();
					}

					if (waypoint != pointPos && checkDist < 300.f)
					{
						dir = checkDir;
						distance = checkDist;
						waypoint = pointPos;
						m_waypoints.clear();
					}

					if (bRenew)
					{
						m_bRot = true;
					}

					dir.Normalize();

					if (bMove && m_bRot)
					{
						m_rotAngle = 0.f;
						m_rotTime = 0.f;

						Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
						m_bRot = false;
						Vec3 crossVec = front.Cross(dir);
						float sign = crossVec.Dot(up);

						if (sign > 0.f)
						{
							m_rotAngle += acosf(front.Dot(dir)) - XM_PI;
						}
						else if (sign < 0.f)
						{
							m_rotAngle -= acosf(front.Dot(dir)) - XM_PI;
						}
					}

					pos += dir * m_finalMovementSpeed * 100.f * DELTATIME;

					m_rotTime += DELTATIME;

					float yAngle = rot.y;

					if (m_rotTime < 0.25f)
						rot.y += m_rotAngle * 4.f * DELTATIME;

					if (_isnanf(rot.y))
						rot.y = yAngle;

					if (bMove && distance < 20.f)
					{
						m_naviMesh->NaviMesh()->SetMove(false);
						bMove = m_naviMesh->NaviMesh()->IsMove();
						m_rotAngle = 0.f;
						m_rotTime = 0.f;
						m_bRot = true;
						m_bMoveMotion = false;

						if (!m_waypoints.empty())
							m_waypoints.pop_front();
					}

					if (!bMove && Vec3::Distance(waypoint, pointPos) > 20.f)
					{
						m_naviMesh->NaviMesh()->SetMove(true);
						m_naviMesh->NaviMesh()->SetRotRenew(true);
					}

					if ((!bMove && Vec3::Distance(waypoint, pointPos) < 20.f) || bRenew)
					{
						m_bFindPath = true;
					}

					// 캐릭터가 가려는 방향 값도 같이 줘서 비교해야할 듯
					if (m_naviMesh->NaviMesh()->IsAboveTile(Object(), dir, m_finalMovementSpeed * 100.f * DELTATIME) && !m_bMotion && !m_bAttack)
					{
						Transform()->SetLocalPos(pos);
						Transform()->SetLocalRot(rot);
					}

					return;

				}
				else if(PLAYER_TYPE::AI == m_playerType)
				{
					if (m_aiPointScript->GetPointType(m_curPoint) == POINT_TYPE::HUNTING && nullptr == m_attackTarget)
					{
						if (!m_listAttackTarget.empty())
						{
							m_attackTarget = m_listAttackTarget.front();
							m_listAttackTarget.pop_front();

							m_listAttackTargetLootingObj.push_back(m_attackTarget);
							m_bFindPath = true;
						}
					}

					if (!m_bMotion && m_bAIMove && nullptr == m_attackTarget)
					{
						m_resetTimer += DELTATIME;

						if (m_resetTimer > 5.f)
						{
							if (m_bAIMove && m_bAIRenew)
							{
								if (!m_curWeaponName.empty())
								{
									switch (m_characterType)
									{
									case CHARACTER_TYPE::AYA:
										SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, true);
										break;
									case CHARACTER_TYPE::HYUNWOO:
										SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::WAIT, true);
										break;
									case CHARACTER_TYPE::LIDAILIN:
										SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::WAIT, true);
										break;
									case CHARACTER_TYPE::END:
										break;
									default:
										break;
									}
								}
								else
									SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_WAIT, true);
							}

							m_bAIMove = false;
							m_resetTimer = 0.f;
						}
					}

					// ai는 캐릭터가 서있는 타일의 주변 타일만 조사해서 갈 수 있는 타일 중에서 랜덤으로 이동
					// 6레벨 전까지는 이동 및 아이템 수집만 하고 그 이후부터 공격적으로 변함 물론 그전에도 공격 받으면 반격
					// 주변 타일 중에서 오브젝트가 있으면 해당 오브젝트로 가서 아이템 수집 또는 플레이어 일 경우 6레벨 이후 공격
					// 공격 타겟이 설정되면 포인트 포즈는 타깃의 위치로 설정
					if (nullptr != m_attackTarget || m_bFindPoint)
					{
						if (!m_bAIMove && m_bFindPath)
						{
							m_curNode = m_naviMesh->NaviMesh()->GetCurNode(Object());

							if (m_bFindPoint)
							{
								m_pointPos = m_aiPointScript->GetPointPos(m_curPoint);
							}
							else
							{
								if (nullptr != m_attackTarget)
								{
									m_pointPos = m_attackTarget->Transform()->GetLocalPos();
								}
								else
								{
									return;
								}
							}

							m_bAIMove = true;
							m_bAIRenew = true;
						}

						pointPos = m_pointPos;
						pointPos.y = Transform()->GetLocalPos().y;

						if (m_bFindPath)
						{
							m_bFindPath = false;
							m_naviMesh->NaviMesh()->FindPath(m_curNode, Object(), pointPos, m_naviMesh->Transform()->GetWorldMatrix(), m_waypoints);
						}

						if (m_waypoints.size() != 0)
						{
							waypoint = m_waypoints.front();
						}
						else
						{
							waypoint = pointPos;
						}

						if (m_bAIMove)
						{
							if (!m_bMoveMotion)
							{
								m_bMoveMotion = true;
							}

							dir.x = waypoint.x - pos.x;
							dir.z = waypoint.z - pos.z;
							distance = dir.Length();
						}

						if (m_bAIMove && m_bAIRenew && !m_bMotion && !m_bAttack)
						{
							if (!m_curWeaponName.empty())
							{
								switch (m_characterType)
								{
								case CHARACTER_TYPE::AYA:
									SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::RUN, true);
									break;
								case CHARACTER_TYPE::HYUNWOO:
									SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::RUN, true);
									break;
								case CHARACTER_TYPE::LIDAILIN:
									SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::RUN, true);
									break;
								case CHARACTER_TYPE::END:
									break;
								default:
									break;
								}
							}
							else
								SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_RUN, true);
						}

						if (m_bAIRenew)
						{
							m_bRot = true;
						}

						dir.Normalize();

						if (m_bAIMove && m_bRot)
						{
							m_rotAngle = 0.f;
							m_rotTime = 0.f;

							Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
							m_bRot = false;
							Vec3 crossVec = front.Cross(dir);
							float sign = crossVec.Dot(up);

							if (sign > 0.f)
							{
								m_rotAngle += acosf(front.Dot(dir)) - XM_PI;
							}
							else if (sign < 0.f)
							{
								m_rotAngle -= acosf(front.Dot(dir)) - XM_PI;
							}
						}

						pos += dir * m_finalMovementSpeed * 100.f * DELTATIME;

						float yAngle = rot.y;

						m_rotTime += DELTATIME;

						if (m_rotTime < 0.25f)
							rot.y += m_rotAngle * 4.f * DELTATIME;

						if (_isnanf(rot.y))
							rot.y = yAngle;

						if (m_bAIMove && distance < 50.f)
						{
							m_bAIMove = false;
							m_rotAngle = 0.f;
							m_rotTime = 0.f;
							m_bRot = true;
							m_bMoveMotion = false;

							if (!m_waypoints.empty())
								m_waypoints.pop_front();
						}

						if (!m_bAIMove && Vec3::Distance(waypoint, pointPos) > 50.f)
						{
							m_bAIMove = true;
							m_bAIRenew = true;
						}

						if (!m_bAIMove && Vec3::Distance(waypoint, pointPos) < 50.f)
						{
							m_bFindPath = true;

							m_bFindPoint = false;
						}
					}
					else if(nullptr == m_attackTarget && !m_bFindPoint && !m_aiPointScript->IsArrive())
					{
						if (!m_bMotion && !m_bAIMove)
						{
							m_curNode = m_naviMesh->NaviMesh()->GetCurNode(Object());
							m_pointPos = AIMove(m_curNode, m_naviMesh->Transform()->GetWorldMatrix());
						}

						pointPos = m_pointPos;

						if (m_bAIMove)
						{
							if (!m_bMoveMotion)
							{
								m_bMoveMotion = true;
							}

							dir.x = pointPos.x - pos.x;
							dir.z = pointPos.z - pos.z;
							distance = dir.Length();
						}

						if (m_bAIRenew)
						{
							m_bRot = true;
						}

						dir.Normalize();

						if (m_bAIMove && m_bRot)
						{
							m_rotAngle = 0.f;
							m_rotTime = 0.f;

							Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
							m_bRot = false;
							Vec3 crossVec = front.Cross(dir);
							float sign = crossVec.Dot(up);

							if (sign > 0.f)
							{
								m_rotAngle += acosf(front.Dot(dir)) - XM_PI;
							}
							else if (sign < 0.f)
							{
								m_rotAngle -= acosf(front.Dot(dir)) - XM_PI;
							}
						}

						pos += dir * m_finalMovementSpeed * 100.f * DELTATIME;

						float yAngle = rot.y;

						m_rotTime += DELTATIME;

						if (m_rotTime < 0.25f)
							rot.y += m_rotAngle * 4.f * DELTATIME;

						if (_isnanf(rot.y))
							rot.y = yAngle;

						if (m_bAIMove && distance < 10.f)
						{
							m_bAIMove = false;
							m_rotAngle = 0.f;
							m_rotTime = 0.f;
							m_bRot = true;
							m_bMoveMotion = false;

							if (m_aiPointScript->GetPointType(m_curPoint) != POINT_TYPE::MOVE)
							{ 
								m_bNext = true;
							}

							m_aiPointScript->SetArrive(true);
							m_aiPointScript->SetPointAction();
						}
					}
				}

				if (nullptr != m_curNode)
				{
					m_curNode->SetMove(false);
				}
			
				// 캐릭터가 가려는 방향 값도 같이 줘서 비교해야할 듯
				if (m_naviMesh->NaviMesh()->IsAboveTile(Object(), dir, m_finalMovementSpeed * 100.f * DELTATIME) && !m_bMotion && !m_bAttack && !m_bAttackMotion)
				{
					if (nullptr != m_attackTarget && m_bAttackDistance)
					{

					}
					else
					{
						Transform()->SetLocalPos(pos);
						Transform()->SetLocalRot(rot);
					}
					
				}
			}
		}
		else
		{
			//공격 애니메이션으로 전환

			m_bMoveMotion = false;
		}
	}

	if (PLAYER_TYPE::AI == m_playerType && m_aiPointScript->GetPointType(m_curPoint) == POINT_TYPE::HUNTING && 
		nullptr == m_aiLootTarget && !m_listAttackTargetLootingObj.empty())
	{
		m_aiLootTarget = m_listAttackTargetLootingObj.front();
		m_listAttackTargetLootingObj.pop_front();
	}

	if (PLAYER_TYPE::AI == m_playerType && nullptr != m_aiLootTarget && !m_bAttack && !m_bSkill_Motion && 
		SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode() && m_listAttackTarget.empty() && nullptr == m_attackTarget)
	{
		Vec3 targetPos = m_aiLootTarget->Transform()->GetWorldPos_XZ();
		Vec3 pos = Transform()->GetWorldPos_XZ();
		float dist = Vec3::Distance(targetPos, pos);

		if (dist < 500.f)
		{
			if (OBJECT_TYPE::INTERACTION == m_aiLootTarget->GetObjectType())
			{
				m_bAIMove = false;
				// 아이템 스폰에 있는 아이템들을 모두 습득
				((CItemSpawnScript*)m_aiLootTarget->GetScripts()[0])->AIGetItems(this, m_arrInventory);
				m_bGetItem = true;
			}
			else if (OBJECT_TYPE::PLAYER == m_aiLootTarget->GetObjectType() && ((CPlayerScript*)m_aiLootTarget->GetScripts()[0])->m_bCharacterDead)
			{
				m_bAIMove = false;
				AILooting((CPlayerScript*)m_aiLootTarget->GetScripts()[0]);
			}
			else if (OBJECT_TYPE::MONSTER == m_aiLootTarget->GetObjectType() && ((CObjectScript*)m_aiLootTarget->GetScripts()[0])->IsCharacterDead())
			{
				m_bAIMove = false;
				AILooting((CMonsterScript*)m_aiLootTarget->GetScripts()[0]);
			}
			else if (!m_aiLootTarget->IsOn())
			{
				m_aiLootTarget = nullptr;
			}
		}
		else
		{
			m_aiLootTarget = nullptr;
		}
	}
}

void CPlayerScript::LvUp()
{
	++m_level;
	++m_skillPoint;

	switch (m_level)
	{
	case 1:
	case 2:
	case 3:
	{
		m_skill_Q_MaxLv = 1;
		m_skill_W_MaxLv = 1;
		m_skill_E_MaxLv = 1;
		m_passive_MaxLV = 1;
		m_skill_R_MaxLv = 0;
	}
	break;
	case 4:
	{
		m_skill_Q_MaxLv = 2;
		m_skill_W_MaxLv = 2;
		m_skill_E_MaxLv = 2;
		m_passive_MaxLV = 1;
		m_skill_R_MaxLv = 0;
	}
	break;
	case 5:
	{
		m_skill_Q_MaxLv = 3;
		m_skill_W_MaxLv = 3;
		m_skill_E_MaxLv = 3;
		m_passive_MaxLV = 2;
		m_skill_R_MaxLv = 0;
	}
	break;
	case 6:
	{
		m_skill_Q_MaxLv = 3;
		m_skill_W_MaxLv = 3;
		m_skill_E_MaxLv = 3;
		m_skill_R_MaxLv = 1;
		m_passive_MaxLV = 2;
	}
	break;
	case 7:
	case 8:
	{
		m_skill_Q_MaxLv = 4;
		m_skill_W_MaxLv = 4;
		m_skill_E_MaxLv = 4;
		m_skill_R_MaxLv = 1;
		m_passive_MaxLV = 2;
	}
	break;
	case 9:
	case 10:
	{
		m_skill_Q_MaxLv = 5;
		m_skill_W_MaxLv = 5;
		m_skill_E_MaxLv = 5;
		m_passive_MaxLV = 3;
		m_skill_R_MaxLv = 1;
	}
	break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	{
		m_skill_Q_MaxLv = 5;
		m_skill_W_MaxLv = 5;
		m_skill_E_MaxLv = 5;
		m_passive_LV = 3;
		m_skill_R_MaxLv = 2;
	}
	break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	{
		m_skill_Q_MaxLv = 5;
		m_skill_W_MaxLv = 5;
		m_skill_E_MaxLv = 5;
		m_passive_LV = 3;
		m_skill_R_MaxLv = 3;
	}
	break;
	default:
		break;
	}
}

void CPlayerScript::SkillLvUp(KEY _key)
{
	switch (_key)
	{
	case KEY::Q:
		if (SkillLvUpCheck(m_skill_Q_Lv, m_skill_Q_MaxLv)) 
		{ 
			++m_skill_Q_Lv; 
			--m_skillPoint;

			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_skill_Q_SM = 70.f + (m_skill_Q_Lv * 10.f);
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_skill_Q_SM = 50.f + (m_skill_Q_Lv * 10.f);
				break;
			default:
				break;
			}
		}
		break;
	case KEY::W:
		if (SkillLvUpCheck(m_skill_W_Lv, m_skill_W_MaxLv)) 
		{
			++m_skill_W_Lv; 
			--m_skillPoint; 

			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_skill_W_SM = 90.f + (m_skill_W_Lv * 10.f);
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_skill_W_SM = 50;
				break;
			default:
				break;
			}
		}
		break;
	case KEY::E:
		if (SkillLvUpCheck(m_skill_E_Lv, m_skill_E_MaxLv)) 
		{
			++m_skill_E_Lv; 
			--m_skillPoint;
			
			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_skill_E_SM = 80.f + (m_skill_E_Lv * 5.f);
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_skill_E_SM = 65.f + (m_skill_E_Lv * 5.f);
				break;
			default:
				break;
			}
		}
		break;
	case KEY::R:
		if (SkillLvUpCheck(m_skill_R_Lv, m_skill_R_MaxLv)) 
		{ 
			++m_skill_R_Lv; 
			--m_skillPoint; 

			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_skill_R_SM = 150.f;
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_skill_R_SM = 150.f;
				break;
			default:
				break;
			}
		}
		break;
	case KEY::T:
		if (SkillLvUpCheck(m_passive_LV, m_passive_MaxLV)) { ++m_passive_LV; --m_skillPoint; }
		break;
	default:
		return;
	}
}

void CPlayerScript::AiSkillLvUp()
{
	if (m_level == 6 || m_level == 11 || m_level == 16)
	{
		SkillLvUp(KEY::R);
		return;
	}

	if (m_skill_Q_Lv < m_skill_Q_MaxLv)
		SkillLvUp(KEY::Q);
	else if (m_skill_W_Lv < m_skill_W_MaxLv)
		SkillLvUp(KEY::W);
	else if (m_skill_E_Lv < m_skill_E_MaxLv)
		SkillLvUp(KEY::E);
	else if (m_passive_LV < m_passive_MaxLV)
		SkillLvUp(KEY::T);
}

void CPlayerScript::AddLootingTree(ITEM _item)
{
	tItem item = CItemManager::GetInst()->GetItemInfo(_item);

	map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(item.ModelName);

	if (m_mapAIItemLoot.end() != iter)
		return;

	if (item.bCombination)
	{
		AddLootingTree(item.Ingredient1ID);
		AddLootingTree(item.Ingredient2ID);
		m_mapAIItemLoot.insert(make_pair(item.ModelName, _item));
	}
	else
	{
		if (m_vecAIStuff.empty())
		{
			tStuff stuff = {};
			stuff.item = item.ID;
			stuff.count = 1;

			m_vecAIStuff.push_back(stuff);
		}
		else
		{
			bool check = true;

			for (size_t i = 0; i < m_vecAIStuff.size(); ++i)
			{
				if (m_vecAIStuff[i].item == item.ID)
				{
					++m_vecAIStuff[i].count;
					check = false;
					break;
				}
			}

			if (check)
			{
				tStuff stuff = {};
				stuff.item = item.ID;
				stuff.count = 1;

				m_vecAIStuff.push_back(stuff);
			}
		}
	}
}

void CPlayerScript::FindMonster(CGameObject* _target)
{
	m_listAttackTarget.push_back(_target);
}

bool CPlayerScript::SkillLvUpCheck(UINT skillLv, UINT skillMaxLv)
{
	return skillLv >= skillMaxLv ? false : true;
}

void CPlayerScript::CooldownTimeCheck()
{
	if (m_skill_Q_CT > 0.f)
		m_skill_Q_CT -= DELTATIME;

	if (m_skill_W_CT > 0.f)
		m_skill_W_CT -= DELTATIME;

	if (m_skill_E_CT > 0.f)
		m_skill_E_CT -= DELTATIME;

	if (m_skill_R_CT > 0.f)
		m_skill_R_CT -= DELTATIME;
}

void CPlayerScript::ResetStat()
{
	m_addAttackPower = 0.f;
	m_addAttackPowerPerLevel = 0.f;
	m_addAttackSpeed = 0.f;
	m_addAttackSpeedPerLevel = 0.f;
	m_addCriticalStrikeChance = 0.f;
	m_addCriticalStrikeDamage = 0.f;
	m_LifeSteal = 0.f;
	m_Omnisyphon = 0.f;
	m_ExtraBaseAttackDamage = 0.f;
	m_ExtraBaseAttackDamagePerLevel = 0.f;
	m_ArmorPenetration = 0.f;
	m_ExtraSkillDamage = 0.f;
	m_ExtraSkillDamagePerLevel = 0.f;
	m_SkillAmplification = 0.f;
	m_SkillAmplificationPerLevel = 0.f;
	m_CooldownReduction = 0.f;
	m_addMaxSP = 0.f;
	m_addSPGen = 0.f;
	m_addSPGen_Percent = 0.f;
	m_addDefense = 0.f;
	m_addDefensePerLevel = 0.f;
	m_addMaxHP = 0.f;
	m_addMaxHPPerLevel = 0.f;
	m_addHPGen = 0.f;
	m_addHPGen_Percent = 0.f;
	m_DefenseFromBasicAttack = 0.f;
	m_SkillDamageReduction = 0.f;
	m_SkillDamageReduction_Percent = 0.f;
	m_CriticalDamageReduction = 0.f;
	m_addMovementSpeed = 0.f;
	m_OutOfCombatMovementSpeed = 0.f;
	m_addVisionRange = 0.f;
	m_addAttackRange = 0.f;
	m_CooldownReductionCap = 0.f;
	m_HealingReceived = 0.f;
}

void CPlayerScript::CalStat()
{
	ResetStat();

	for (UINT i = 0; i < (UINT)EQUIP_TYPE::END; ++i)
	{
		if (ITEM::END == m_arrEquipItems[i])
			continue;

		tItem& item = CItemManager::GetInst()->GetItemInfo(m_arrEquipItems[i]);

		if ((EQUIP_TYPE)i == EQUIP_TYPE::WEAPON)
		{
			SetWeaponName(item.Type);
		}

		m_addAttackPower += item.AttackPower;
		m_addAttackPowerPerLevel += item.AttackPowerPerLevel;
		m_addAttackSpeed += item.AttackSpeed;
		m_addAttackSpeedPerLevel += item.AttackSpeedPerLevel;
		m_addCriticalStrikeChance += item.CriticalStrikeChance;
		m_addCriticalStrikeDamage += item.CriticalStrikeDamage;
		m_LifeSteal += item.LifeSteal;
		m_Omnisyphon += item.Omnisyphon;
		m_ExtraBaseAttackDamage += item.ExtraBaseAttackDamage;
		m_ExtraBaseAttackDamagePerLevel += item.ExtraBaseAttackDamagePerLevel;
		m_ArmorPenetration += item.ArmorPenetration;
		m_ExtraSkillDamage += item.ExtraSkillDamage;
		m_ExtraSkillDamagePerLevel += item.ExtraSkillDamagePerLevel;
		m_SkillAmplification += item.SkillAmplification;
		m_SkillAmplificationPerLevel += item.SkillAmplificationPerLevel;
		m_CooldownReduction += item.CooldownReduction;
		m_addMaxSP += item.MaxSP;
		m_addSPGen += item.SPGen;
		m_addSPGen_Percent += item.SPGen_Percent;
		m_addDefense += item.Defense;
		m_addDefensePerLevel += item.DefensePerLevel;
		m_addMaxHP += item.MaxHP;
		m_addMaxHPPerLevel += item.MaxHPPerLevel;
		m_addHPGen += item.HPGen;
		m_addHPGen_Percent += item.HPGen_Percent;
		m_DefenseFromBasicAttack += item.DefenseFromBasicAttack;
		m_SkillDamageReduction += item.SkillDamageReduction;
		m_SkillDamageReduction_Percent += item.SkillDamageReduction_Percent;
		m_CriticalDamageReduction += item.CriticalDamageReduction;
		m_addMovementSpeed += item.MovementSpeed;
		m_OutOfCombatMovementSpeed += item.OutOfCombatMovementSpeed;
		m_addVisionRange += item.VisionRange;
		m_addAttackRange += item.AttackRange;
		m_CooldownReductionCap += item.CooldownReductionCap;
		m_HealingReceived += item.HealingReceived;
	}

	m_finalMaxHP = m_maxHP + m_addMaxHP + (m_addMaxHPPerLevel + m_level);
	m_finalMaxSP = m_maxStamina + m_addMaxSP;
	m_finalAttackPower = m_offense + m_addAttackPower + (m_addAttackPowerPerLevel * m_level);
	m_finalDefense = m_defense + m_addDefense + (m_addDefensePerLevel * m_level);
	m_finalHPGen = m_healthRegen + m_addHPGen + m_HealingReceived + ((m_healthRegen + m_addHPGen) * m_addHPGen_Percent);
	m_finalSPGen = m_staminaRegen + m_addSPGen + ((m_staminaRegen + m_addSPGen) * m_addSPGen_Percent);
	m_finalAttackSpeed = m_attackSpeed + m_addAttackSpeed + (m_addAttackSpeedPerLevel * m_level);
	m_midMovementSpeed = m_moveSpeed + m_addMovementSpeed;
	m_finalVisionRange = m_FOV + m_addVisionRange;
	m_finalAttackRange = m_attackRange + m_addAttackRange;
	m_finalCriticalChance = m_criticalChance + m_addCriticalStrikeChance;
	m_finalCriticalDamage = m_criticalDamage + m_addCriticalStrikeDamage;

	if (m_curHP > m_finalMaxHP)
	{
		m_curHP = m_finalMaxHP;
	}

	if (m_curStamina > m_finalMaxSP)
	{
		m_curStamina = m_finalMaxSP;
	}

	if (nullptr != m_attackBox)
	{
		((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetCritical(m_finalCriticalDamage);
		((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetCriticalChance(m_finalCriticalChance);
	}
	
}

Vec3 CPlayerScript::AIMove(CNaviTileNode* _curNode, Matrix _worldMatrix)
{
	if (nullptr == _curNode)
		return Vec3();

	m_bAIMove = true;
	m_bAIRenew = true;
	m_aiLootTarget = nullptr;

	if (!m_curWeaponName.empty())
	{
		switch (m_characterType)
		{
		case CHARACTER_TYPE::AYA:
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::RUN, true);
			break;
		case CHARACTER_TYPE::HYUNWOO:
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::RUN, true);
			break;
		case CHARACTER_TYPE::LIDAILIN:
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::RUN, true);
			break;
		case CHARACTER_TYPE::END:
			break;
		default:
			break;
		}
	}
	else
		SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_RUN, true);

	Vec3 destPos = m_aiPointScript->GetPointPos(m_curPoint);

	// 해당 타일 위에 오브젝트가 있을 때 해당 오브젝트가 플레이어 인지 아니면 아이템 스폰인지 알아야됨
	// 캐릭터가 6레벨 이상이면 플레이어를 타겟으로 삼음.
	Vec3 pos = Transform()->GetLocalPos();

	if (6 < m_level)
	{
		const vector<CGameObject*>& vecPlayer = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);

		for (size_t i = 0; i < vecPlayer.size(); ++i)
		{
			Vec3 targetPos = vecPlayer[i]->Transform()->GetLocalPos();
			if (Object() != vecPlayer[i] && !((CObjectScript*)vecPlayer[i]->GetScripts()[0])->IsCharacterDead() && Vec3::Distance(pos, targetPos) < m_FOV * 100.f)
			{
				m_attackTarget = vecPlayer[i];
				m_aiLootTarget = vecPlayer[i];
				break;
			}
		}
	}

	return destPos;
}

void CPlayerScript::AIFoodOrBeverage()
{
	if (PLAYER_TYPE::AI == m_playerType)
	{
		float HPRate = m_curHP / m_finalMaxHP;
		float SPRate = m_curStamina / m_finalMaxSP;

		if (HPRate < 0.3f && !m_bFood)
		{
			m_bFood = true;

			for (int i = 0; i < 10; ++i)
			{
				tItem& food = CItemManager::GetInst()->GetItemInfo(m_arrInventory[i].ID);

				if (ITEM_TYPE::FOOD == food.Type)
				{
					// 음식으로 인한 체젠은 18초에 걸쳐 이루어 지므로 1/18 의 값만 받아 18초 동안 작동한다.
					m_foodHPGen = food.HPGen / 18.f;
					m_arrInventory[i].count -= 1;
				}
			}
		}

		if (SPRate < 0.3f && !m_bBeverage)
		{
			m_bBeverage = true;

			for (int i = 0; i < 10; ++i)
			{
				tItem& beverage = CItemManager::GetInst()->GetItemInfo(m_arrInventory[i].ID);

				if (ITEM_TYPE::BEVERAGE == beverage.Type)
				{
					// 음식으로 인한 스젠은 18초에 걸쳐 이루어 지므로 1/18 의 값만 받아 18초 동안 작동한다.
					m_foodSPGen = beverage.SPGen / 18.f;
					m_arrInventory[i].count -= 1;
				}
			}
		}
	}
}

void CPlayerScript::SetWeaponName(ITEM_TYPE _eType)
{
	switch (_eType)
	{
	case ITEM_TYPE::DAGGER:
		m_curWeaponName = L"Dagger";
		break;
	case ITEM_TYPE::GLOVES:
		m_curWeaponName = L"Glove";
		break;
	case ITEM_TYPE::NUNCHAKU:
		m_curWeaponName = L"Nunchaku";
		break;
	case ITEM_TYPE::TONFA:
		m_curWeaponName = L"Tonfa";
		break;
	case ITEM_TYPE::BAT:
		m_curWeaponName = L"Bat";
		break;
	case ITEM_TYPE::WHIP:
		m_curWeaponName = L"Whip";
		break;
	case ITEM_TYPE::HAMMER:
		m_curWeaponName = L"Hammer";
		break;
	case ITEM_TYPE::THROW:
		m_curWeaponName = L"Throw";
		break;
	case ITEM_TYPE::SHURIKEN:
		m_curWeaponName = L"Shuriken";
		break;
	case ITEM_TYPE::BOW:
		m_curWeaponName = L"bow";
		break;
	case ITEM_TYPE::CROSSBOW:
		m_curWeaponName = L"CrossBow";
		break;
	case ITEM_TYPE::PISTOL:
		m_curWeaponName = L"Pistol";
		break;
	case ITEM_TYPE::ASSAULT_RIFLE:
		m_curWeaponName = L"Gun";
		break;
	case ITEM_TYPE::SNIPER_RIFLE:
		m_curWeaponName = L"Sniper";
		break;
	case ITEM_TYPE::AXE:
		m_curWeaponName = L"Axe";
		break;
	case ITEM_TYPE::TWO_HANDED_SWORD:
		m_curWeaponName = L"2Handed";
		break;
	case ITEM_TYPE::DUAL_SWORD:
		m_curWeaponName = L"Dual";
		break;
	case ITEM_TYPE::SPEAR:
		m_curWeaponName = L"Spear";
		break;
	case ITEM_TYPE::RAPIER:
		m_curWeaponName = L"Rapier";
		break;
	case ITEM_TYPE::CHEST:
		m_curWeaponName = L"Chest";
		break;
	case ITEM_TYPE::END:
		m_curWeaponName = L"BareHands";
		break;
	default:
		break;
	}
}

void CPlayerScript::PlayerItemCombination()
{
	vector<ITEM> vecTempInventory;
	ITEM result;
	ITEM ingredient1;
	ITEM ingredient2;

	for (int i = 0; i < 10; ++i)
	{
		vecTempInventory.push_back(m_arrInventory[i].ID);
	}

	for (int i = 0; i < 6; ++i)
	{
		vecTempInventory.push_back(m_arrEquipItems[i]);
	}

	for (size_t i = 0; i < vecTempInventory.size() - 1; ++i)
	{
		if (ITEM::END == vecTempInventory[i])
			continue;

		bool check = false;
		for (size_t j = i + 1; j < vecTempInventory.size(); ++j)
		{
			if (ITEM::END == vecTempInventory[j])
				continue;

			// 루트 시스템이 없으므로 인벤토리에 있는 아이템을 조사하여 조합 할 수 있는 아이템을 조합
			ITEM item = CItemManager::GetInst()->ItemCombination(vecTempInventory[i], vecTempInventory[j]);

			if (item == ITEM::END)
				continue;

			ingredient1 = vecTempInventory[i];
			ingredient2 = vecTempInventory[j];

			tItem& combineItem = CItemManager::GetInst()->GetItemInfo(item);

			map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(combineItem.ModelName);

			if (m_mapAIItemLoot.end() == iter)
				continue;

			if (!m_bCook && !m_bCrafting)
			{
				m_bMoveMotion = false;
				m_naviMesh->NaviMesh()->SetRenew(false);
				m_naviMesh->NaviMesh()->SetMove(false);

				if (combineItem.Type == ITEM_TYPE::FOOD || combineItem.Type == ITEM_TYPE::BEVERAGE)
				{
					if (!m_bCook && !m_bCrafting && combineItem.Type == ITEM_TYPE::FOOD)
					{
						m_bCook = true;
						m_bMotion = true;

						SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_CRAFTFOOD, false);
						if (nullptr == m_craftFoodSound.Get() && IsPlaySound())
						{
							m_craftFoodSound->Play(1, 0.1f);
						}

						if (nullptr != m_flyingpan)
						{
							m_flyingpan->SetObjOn();

							if (nullptr != m_weapon)
								m_weapon->SetObjOff();

							CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_flyingpan->GetScripts()[0];
							m_EquipItemMaterial = IgItemScript->GetCurMaterial();
							Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
						}
						if (nullptr != m_craftToolFood)
						{
							m_craftToolFood->SetObjOn();
						}
					}
				}
				else
				{
					if (!m_bCook && !m_bCrafting)
					{
						m_bCrafting = true;
						m_bMotion = true;
						// 장비 크래프팅 모션
						SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_CRAFTMETAL, false);

						if (nullptr != m_weapon && m_weapon->IsOn())
						{
							m_weapon->SetObjOff();
							Animator3D()->SetBoneMaterial(nullptr);
						}

						if (nullptr != m_craftToolMetal)
						{
							m_craftToolMetal->SetObjOn();
							CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_craftToolMetal->GetScripts()[0];
							IgItemScript->SetAppearMotion(false, Animator3D()->GetAnimSpeed());
						}

						if (nullptr == m_craftToolSound.Get() && IsPlaySound() && IsPlaySound())
						{
							m_craftToolSound->Play(1, 0.1f);
						}
					}
				}

				// 아이템 등급에 따른 애니메이션 재생 속도 조절
				if (ITEM_GRADE::GRAY == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(2.f);
				}
				else if (ITEM_GRADE::GREEN == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.8f);
				}
				else if (ITEM_GRADE::BLUE == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.6f);
				}
				else if (ITEM_GRADE::PURPLE == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.4f);
				}
				else if (ITEM_GRADE::YELLOW == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.2f);
				}

				m_craftGrade = combineItem.Grade;
			}

			if (m_bCook || m_bCrafting)
				m_bMotion = true;

			if (Animator3D()->IsAnimFinish() && (m_bCook || m_bCrafting)) // 음식 만드는 모션이나 장비 만드는 모션이 끝났을 때
			{
				result = combineItem.ID;
				check = true;
				break;
			}
		}

		if (check)
			break;
	}

	if (Animator3D()->IsAnimFinish() && (m_bCook || m_bCrafting)) // 음식 만드는 모션이나 장비 만드는 모션이 끝났을 때
	{
		m_bMotion = false;
		bool bSameCheck = false;

		bool bInInven1 = false;
		bool bInInven2 = false;
		bool bInEquip1 = false;
		bool bInEquip2 = false;

		int invenIndex1 = 0;
		int invenIndex2 = 0;
		int equipIndex1 = 0;
		int equipIndex2 = 0;

		for (int i = 0; i < 10; ++i)
		{
			if (m_arrInventory[i].ID == ingredient1)
			{
				bInInven1 = true;
				invenIndex1 = i;
			}
		}

		for (int i = 0; i < 10; ++i)
		{
			if (m_arrInventory[i].ID == ingredient2)
			{
				bInInven2 = true;
				invenIndex2 = i;
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			if (m_arrEquipItems[i] == ingredient1)
			{
				bInEquip1 = true;
				equipIndex1 = i;
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			if (m_arrEquipItems[i] == ingredient2)
			{
				bInEquip2 = true;
				equipIndex2 = i;
			}
		}

		// 인벤토리에 같은 아이템이 있는지 조사
		// ex) 제작된 아이템 중 인벤토리에 중복된 아이템이 있는지 체크
		for (int k = 0; k < 10; ++k)
		{
			if (ITEM::END != result && m_arrInventory[k].ID == result)
			{
				tItem& itemInfo = CItemManager::GetInst()->GetItemInfo(m_arrInventory[k].ID);

				if (m_arrInventory[k].count < itemInfo.MaxQuantity)
				{
					if (bInInven1)
					{
						m_arrInventory[invenIndex1].count -= 1;

						if (m_arrInventory[invenIndex1].count == 0)
							m_arrInventory[invenIndex1].ID = ITEM::END;
					}

					if (bInInven2)
					{
						m_arrInventory[invenIndex2].count -= 1;

						if (m_arrInventory[invenIndex2].count == 0)
							m_arrInventory[invenIndex2].ID = ITEM::END;
					}

					if (bInEquip1)
					{
						m_arrEquipItems[equipIndex1] = ITEM::END;
					}

					if (bInEquip2)
					{
						m_arrEquipItems[equipIndex2] = ITEM::END;
					}

					m_arrInventory[k].ID = result;
					m_arrInventory[k].count += 1;
					bSameCheck = true;
				}

				break;
			}
		}

		if (Animator3D()->IsAnimFinish() && m_bCook || m_bCrafting)
		{
			m_bEquipChange = true;

			if (m_bCook)
			{
				m_bCook = false;
				m_craftToolFood->SetObjOff();

			}
			if (m_bCrafting)
			{
				m_bCrafting = false;
				m_craftHammer->SetObjOff();
				m_craftDriver->SetObjOff();

				if (nullptr != m_craftToolMetal)
				{
					m_craftToolMetal->Animator3D()->Reset();
					m_craftToolMetal->SetObjOff();
				}
			}

			if (nullptr != m_weapon)
			{
				m_weapon->SetObjOn();

				CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_weapon->GetScripts()[0];
				m_EquipItemMaterial = IgItemScript->GetCurMaterial();
				Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
			}
		}

		wstring itemName = CItemManager::GetInst()->GetItemInfo(result).ModelName;
		map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(itemName);
		iter = m_mapAIItemLoot.erase(iter);

		if (ITEM::END != result && !bSameCheck)
		{
			if (bInInven1 && bInInven2)
			{
				if (m_arrInventory[invenIndex1].count == 1 && m_arrInventory[invenIndex2].count == 1)
				{
					m_arrInventory[invenIndex1].ID = result;
					m_arrInventory[invenIndex2].ID = ITEM::END;
				}
				else if (m_arrInventory[invenIndex1].count == 1 && m_arrInventory[invenIndex2].count > 1)
				{
					m_arrInventory[invenIndex1].ID = result;
					m_arrInventory[invenIndex2].count -= 1;
				}
				else if (m_arrInventory[invenIndex1].count > 1 && m_arrInventory[invenIndex2].count == 1)
				{
					m_arrInventory[invenIndex1].count -= 1;
					m_arrInventory[invenIndex2].ID = result;
				}
				else if (m_arrInventory[invenIndex1].count > 1 && m_arrInventory[invenIndex2].count > 1)
				{
					for (int k = 0; k < 10; ++k)
					{
						if (m_arrInventory[k].ID == ITEM::END)
						{
							m_arrInventory[invenIndex1].count -= 1;
							m_arrInventory[invenIndex2].count -= 1;
							m_arrInventory[k].ID = result;
							m_arrInventory[k].count = 1;
							break;
						}
					}
				}
			}
			else if ((bInInven1 && bInEquip1 && !bInInven2 && !bInEquip2) || (bInInven2 && bInEquip1 && !bInInven1 && !bInEquip2) ||
				(bInInven1 && bInEquip2 && !bInInven2 && !bInEquip1) || (bInInven2 && bInEquip2 && !bInInven1 && !bInEquip1))
			{
				int left;
				int right;

				if (bInInven1)
					left = invenIndex1;
				else
					left = invenIndex2;

				if (bInEquip1)
					right = equipIndex1;
				else
					right = equipIndex2;

				if (m_arrInventory[left].count == 1)
				{
					m_arrInventory[left].ID = result;
					m_arrEquipItems[right] = ITEM::END;
				}
				else if (m_arrInventory[left].count > 1)
				{
					for (int k = 0; k < 10; ++k)
					{
						if (m_arrInventory[k].ID == ITEM::END)
						{
							m_arrInventory[left].count -= 1;
							m_arrEquipItems[right] = ITEM::END;
							m_arrInventory[k].ID = result;
							m_arrInventory[k].count = 1;
							break;
						}
					}
				}
			}
			else if (bInEquip1 && bInEquip2)
			{
				for (int k = 0; k < 10; ++k)
				{
					if (m_arrInventory[k].ID == ITEM::END)
					{
						m_arrEquipItems[equipIndex1] = ITEM::END;
						m_arrEquipItems[equipIndex2] = ITEM::END;
						m_arrInventory[k].ID = result;
						m_arrInventory[k].count = 1;
						break;
					}
				}
			}
		}
	}

	if (Animator3D()->IsAnimFinish() && (m_bCook || m_bCrafting))
	{
		if (m_bCook)
		{
			m_bCook = false;
			m_craftToolFood->SetObjOff();
		}
		if (m_bCrafting)
		{
			m_bCrafting = false;
			m_craftHammer->SetObjOff();
			m_craftDriver->SetObjOff();

			if (nullptr != m_craftToolMetal)
			{
				m_craftToolMetal->Animator3D()->Reset();
				m_craftToolMetal->SetObjOff();
			}
		}

		if (nullptr != m_weapon)
		{
			m_weapon->SetObjOn();

			CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_weapon->GetScripts()[0];
			m_EquipItemMaterial = IgItemScript->GetCurMaterial();
			Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
		}

		switch (m_craftGrade)
		{
		case ITEM_GRADE::NONE:
		case ITEM_GRADE::GRAY:
		case ITEM_GRADE::GREEN:
		{
			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_craftUncommon");
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Hyunwoo_craftUncommon");
				break;
			case CHARACTER_TYPE::LIDAILIN:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_craftUncommon");
				break;
			default:
				break;
			}
		}
			break;
		case ITEM_GRADE::BLUE:
			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_craftRare");
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Hyunwoo_craftRare");
				break;
			case CHARACTER_TYPE::LIDAILIN:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_craftRare");
				break;
			default:
				break;
			}
			break;
		case ITEM_GRADE::PURPLE:
			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_craftEpic");
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Hyunwoo_craftEpic");
				break;
			case CHARACTER_TYPE::LIDAILIN:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_craftEpic");
				break;
			default:
				break;
			}
			break;
		case ITEM_GRADE::YELLOW:
			switch (m_characterType)
			{
			case CHARACTER_TYPE::AYA:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_craftLegend");
				break;
			case CHARACTER_TYPE::HYUNWOO:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"Hyunwoo_craftLegend");
				break;
			case CHARACTER_TYPE::LIDAILIN:
				m_craftCompleteSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_craftLegend");
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		if (nullptr != m_craftCompleteSound.Get() && IsPlaySound())
			m_craftCompleteSound->Play(1, 0.1f);

		m_craftGrade = ITEM_GRADE::NONE;
	}
}

void CPlayerScript::AIItemCombination()
{
	vector<bool> vecCraft;

	vector<ITEM> vecTempInventory;
	ITEM result;
	ITEM ingredient1;
	ITEM ingredient2;

	for (int i = 0; i < 10; ++i)
	{
		vecTempInventory.push_back(m_arrInventory[i].ID);
	}

	for (int i = 0; i < 6; ++i)
	{
		vecTempInventory.push_back(m_arrEquipItems[i]);
	}

	for (size_t i = 0; i < vecTempInventory.size() - 1; ++i)
	{
		if (ITEM::END == vecTempInventory[i])
			continue;

		bool check = false;
		for (size_t j = i + 1; j < vecTempInventory.size(); ++j)
		{
			if (ITEM::END == vecTempInventory[j])
				continue;

			// 루트 시스템이 없으므로 인벤토리에 있는 아이템을 조사하여 조합 할 수 있는 아이템을 조합
			ITEM item = CItemManager::GetInst()->ItemCombination(vecTempInventory[i], vecTempInventory[j]);

			if (item == ITEM::END)
				continue;

			vecCraft.push_back(true);

			ingredient1 = vecTempInventory[i];
			ingredient2 = vecTempInventory[j];

			tItem& combineItem = CItemManager::GetInst()->GetItemInfo(item);

			map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(combineItem.ModelName);

			if (m_mapAIItemLoot.end() == iter)
				continue;

			if (!m_bCook && !m_bCrafting)
			{
				//만드려는 것이 음식인 경우
				if (combineItem.Type == ITEM_TYPE::FOOD || combineItem.Type == ITEM_TYPE::BEVERAGE)
				{
					m_bCook = true;

					if (nullptr != m_flyingpan)
					{
						m_flyingpan->SetObjOn();
						m_weapon->SetObjOff();

						CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_flyingpan->GetScripts()[0];
						m_EquipItemMaterial = IgItemScript->GetCurMaterial();
						Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
					}

					SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_CRAFTFOOD, false);
					// 음식 만드는 모션
				}
				else
				{
					// 장비인 경우
					m_bCrafting = true;

					if (nullptr != m_craftHammer)
					{
						m_craftHammer->SetObjOn();

						if(nullptr != m_weapon)
							m_weapon->SetObjOff();

						CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_craftHammer->GetScripts()[0];
						m_EquipItemMaterial = IgItemScript->GetCurMaterial();
						Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
					}

					// 장비 크래프팅 모션
					SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_CRAFTMETAL, false);
				}

				// 아이템 등급에 따른 애니메이션 재생 속도 조절
				if (ITEM_GRADE::GRAY == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(2.f);
				}
				else if (ITEM_GRADE::GREEN == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.8f);
				}
				else if (ITEM_GRADE::BLUE == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.6f);
				}
				else if (ITEM_GRADE::PURPLE == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.4f);
				}
				else if (ITEM_GRADE::YELLOW == combineItem.Grade)
				{
					Animator3D()->SetAnimSpeed(1.2f);
				}

				if (nullptr != m_craftToolMetal && m_bCrafting)
				{
					m_craftToolMetal->SetObjOn();
					CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_craftToolMetal->GetScripts()[0];
					IgItemScript->SetAppearMotion(false, Animator3D()->GetAnimSpeed());
				}
			}

			if (m_bCook || m_bCrafting)
			{
				m_bMotion = true;
				m_bAIMove = false;
			}

			if (Animator3D()->IsAnimFinish() && (m_bCook || m_bCrafting)) // 음식 만드는 모션이나 장비 만드는 모션이 끝났을 때
			{
				result = combineItem.ID;
				check = true;
				break;
			}
		}

		if (check)
			break;
	}
	
	if (Animator3D()->IsAnimFinish() &&	(m_bCook || m_bCrafting)) // 음식 만드는 모션이나 장비 만드는 모션이 끝났을 때
	{
		m_bMotion = false;
		bool bSameCheck = false;

		bool bInInven1 = false;
		bool bInInven2 = false;
		bool bInEquip1 = false;
		bool bInEquip2 = false;

		int invenIndex1 = 0;
		int invenIndex2 = 0;
		int equipIndex1 = 0;
		int equipIndex2 = 0;

		for (int i = 0; i < 10; ++i)
		{
			if (m_arrInventory[i].ID == ingredient1)
			{
				bInInven1 = true;
				invenIndex1 = i;
			}
		}

		for (int i = 0; i < 10; ++i)
		{
			if (m_arrInventory[i].ID == ingredient2)
			{
				bInInven2 = true;
				invenIndex2 = i;
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			if (m_arrEquipItems[i] == ingredient1)
			{
				bInEquip1 = true;
				equipIndex1 = i;
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			if (m_arrEquipItems[i] == ingredient2)
			{
				bInEquip2 = true;
				equipIndex2 = i;
			}
		}

		// 인벤토리에 같은 아이템이 있는지 조사
		// ex) 제작된 아이템 중 인벤토리에 중복된 아이템이 있는지 체크
		for (int k = 0; k < 10; ++k)
		{
			if (ITEM::END != result && m_arrInventory[k].ID == result)
			{
				tItem& itemInfo = CItemManager::GetInst()->GetItemInfo(m_arrInventory[k].ID);

				if (m_arrInventory[k].count < itemInfo.MaxQuantity)
				{
					if (bInInven1)
					{
						m_arrInventory[invenIndex1].count -= 1;

						if (m_arrInventory[invenIndex1].count == 0)
							m_arrInventory[invenIndex1].ID = ITEM::END;
					}

					if (bInInven2)
					{
						m_arrInventory[invenIndex2].count -= 1;

						if (m_arrInventory[invenIndex2].count == 0)
							m_arrInventory[invenIndex2].ID = ITEM::END;
					}

					if (bInEquip1)
					{
						m_arrEquipItems[equipIndex1] = ITEM::END;
					}

					if (bInEquip2)
					{
						m_arrEquipItems[equipIndex2] = ITEM::END;
					}
					
					m_arrInventory[k].ID = result;
					m_arrInventory[k].count += 1;
					bSameCheck = true;
				}

				break;
			}
		}

		if (Animator3D()->IsAnimFinish() && m_bCook || m_bCrafting)
		{
			if (m_bCook)
			{
				m_bCook = false;
				m_craftToolFood->SetObjOff();

			}
			if (m_bCrafting)
			{
				m_bCrafting = false;
				m_craftHammer->SetObjOff();
				m_craftDriver->SetObjOff();

				if (nullptr != m_craftToolMetal)
				{
					m_craftToolMetal->Animator3D()->Reset();
					m_craftToolMetal->SetObjOff();
				}
			}

			if (nullptr != m_weapon)
			{
				m_weapon->SetObjOn();

				CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_weapon->GetScripts()[0];
				m_EquipItemMaterial = IgItemScript->GetCurMaterial();
				Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
			}
		}

		wstring itemName = CItemManager::GetInst()->GetItemInfo(result).ModelName;
		map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(itemName);
		iter = m_mapAIItemLoot.erase(iter);

		if (ITEM::END != result && !bSameCheck)
		{
			if (bInInven1 && bInInven2)
			{
				if (m_arrInventory[invenIndex1].count == 1 && m_arrInventory[invenIndex2].count == 1)
				{
					m_arrInventory[invenIndex1].ID = result;
					m_arrInventory[invenIndex2].ID = ITEM::END;
				}
				else if (m_arrInventory[invenIndex1].count == 1 && m_arrInventory[invenIndex2].count > 1)
				{
					m_arrInventory[invenIndex1].ID = result;
					m_arrInventory[invenIndex2].count -= 1;
				}
				else if (m_arrInventory[invenIndex1].count > 1 && m_arrInventory[invenIndex2].count == 1)
				{
					m_arrInventory[invenIndex1].count -= 1;
					m_arrInventory[invenIndex2].ID = result;
				}
				else if (m_arrInventory[invenIndex1].count > 1 && m_arrInventory[invenIndex2].count > 1)
				{
					for (int k = 0; k < 10; ++k)
					{
						if (m_arrInventory[k].ID == ITEM::END)
						{
							m_arrInventory[invenIndex1].count -= 1;
							m_arrInventory[invenIndex2].count -= 1;
							m_arrInventory[k].ID = result;
							m_arrInventory[k].count = 1;
							break;
						}
					}
				}
			}
			else if ((bInInven1 && bInEquip1 && !bInInven2 && !bInEquip2) || (bInInven2 && bInEquip1 && !bInInven1 && !bInEquip2) ||
				(bInInven1 && bInEquip2 && !bInInven2 && !bInEquip1) || (bInInven2 && bInEquip2 && !bInInven1 && !bInEquip1))
			{
				int left;
				int right;

				if (bInInven1)
					left = invenIndex1;
				else
					left = invenIndex2;

				if (bInEquip1)
					right = equipIndex1;
				else
					right = equipIndex2;

				if (m_arrInventory[left].count == 1)
				{
					m_arrInventory[left].ID = result;
					m_arrEquipItems[right] = ITEM::END;
				}
				else if (m_arrInventory[left].count > 1)
				{
					for (int k = 0; k < 10; ++k)
					{
						if (m_arrInventory[k].ID == ITEM::END)
						{
							m_arrInventory[left].count -= 1;
							m_arrEquipItems[right] = ITEM::END;
							m_arrInventory[k].ID = result;
							m_arrInventory[k].count = 1;
							break;
						}
					}
				}
			}
			else if (bInEquip1 && bInEquip2)
			{
				for (int k = 0; k < 10; ++k)
				{
					if (m_arrInventory[k].ID == ITEM::END)
					{
						m_arrEquipItems[equipIndex1] = ITEM::END;
						m_arrEquipItems[equipIndex2] = ITEM::END;
						m_arrInventory[k].ID = result;
						m_arrInventory[k].count = 1;
						break;
					}
				}
			}
		}
	}

	if (vecCraft.empty())
		m_bGetItem = false;

	for (UINT i = 0; i < (UINT)EQUIP_TYPE::END; ++i)
	{
		AIEquipChange((EQUIP_TYPE)i);
	}
}

void CPlayerScript::Skill()
{
	if (KEY_TAP(KEY::Q) && KEY_NONE(KEY::LCTRL) && 0 < m_skill_Q_Lv)
	{
		//스킬 쿨 타임일 때 해당 버튼 눌렀을 경우 소리를 나오게 하려면
		//안쪽에서 예외처리
		if (m_skill_Q_CT > 0.f || m_skill_Q_SM > m_curStamina)
		{
			//쿨타임일때 사운드
		}
		else
		{
			Skill_Q(m_attackTarget);
		}

	}
	else if (KEY_TAP(KEY::W) && KEY_NONE(KEY::LCTRL) && 0 < m_skill_W_Lv)
	{
		if (m_skill_W_CT > 0.f || m_skill_W_SM > m_curStamina)
		{
		}
		else
		{
			Skill_W(m_attackTarget);
		}
	}
	else if (KEY_TAP(KEY::E) && KEY_NONE(KEY::LCTRL) && 0 < m_skill_E_Lv)
	{
		if (m_skill_E_CT > 0.f || m_skill_E_SM > m_curStamina)
		{
		}
		else
		{
			Skill_E(m_attackTarget);
		}
	}
	else if (KEY_HOLD(KEY::R) && KEY_NONE(KEY::LCTRL) && 0 < m_skill_R_Lv)
	{
		if (m_skill_R_CT > 0.f || m_skill_R_SM > m_curStamina)
		{
		}
		else
		{
			Skill_R(m_attackTarget);
		}
	}
}

void CPlayerScript::AISkill()
{
	if (nullptr != m_attackTarget)
	{
		Vec3 targetPos = m_attackTarget->Transform()->GetLocalPos();
		Vec3 pos = Transform()->GetLocalPos();
		float dist = Vec3::Distance(pos, targetPos);

		if (dist < m_attackRange)
		{
			if (0 < m_skill_Q_Lv)
			{
				//스킬 쿨 타임일 때 해당 버튼 눌렀을 경우 소리를 나오게 하려면
				//안쪽에서 예외처리

				if (m_skill_Q_CT <= 0.f && m_skill_Q_SM < m_curStamina)
				{
					Skill_Q(m_attackTarget);
					return;
				}

			}
			if (0 < m_skill_W_Lv)
			{
				if (m_skill_W_CT <= 0.f && m_skill_W_SM < m_curStamina)
				{
					Skill_W(m_attackTarget);
					return;
				}
			}
			if (0 < m_skill_E_Lv)
			{
				if (m_skill_E_CT <= 0.f && m_skill_E_SM < m_curStamina)
				{
					Skill_E(m_attackTarget);
					return;
				}
			}
			if (0 < m_skill_R_Lv)
			{
				if (m_skill_R_CT <= 0.f && m_skill_R_SM < m_curStamina)
				{
					Skill_R(m_attackTarget);
					return;
				}
			}
		}
	}
}

void CPlayerScript::AILooting(CPlayerScript* _target)
{
	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END != m_arrInventory[i].ID)
			continue;

		for (int j = 0; j < 10; ++j)
		{
			if (ITEM::END == _target->m_arrInventory[j].ID)
				continue;

			bool check = false;
			for (size_t k = 0; k < m_vecAIStuff.size(); ++k)
			{
				if (0 == m_vecAIStuff[k].count)
					continue;

				if (_target->m_arrInventory[j].ID == m_vecAIStuff[k].item)
				{
					m_arrInventory[i].ID = _target->m_arrInventory[j].ID;
					m_arrInventory[i].count = 1;
					m_bGetItem = true;
					--m_vecAIStuff[k].count;
					check = true;
					break;
				}
			}

			if (!check)
			{
				tItem item = CItemManager::GetInst()->GetItemInfo(_target->m_arrInventory[j].ID);
				map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(item.ModelName);

				if (m_mapAIItemLoot.end() != iter)
				{
					m_arrInventory[i].ID = _target->m_arrInventory[j].ID;
					m_arrInventory[i].count = 1;
					m_bGetItem = true;
					iter = m_mapAIItemLoot.erase(iter);
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END != m_arrInventory[i].ID)
			continue;

		for (int j = 0; j < 6; ++j)
		{
			if (ITEM::END == _target->m_arrEquipItems[j])
				continue;

			bool check = false;
			for (size_t k = 0; k < m_vecAIStuff.size(); ++k)
			{
				if (0 == m_vecAIStuff[k].count)
					continue;

				if (_target->m_arrEquipItems[j] == m_vecAIStuff[k].item)
				{
					m_arrInventory[i].ID = _target->m_arrEquipItems[j];
					m_arrInventory[i].count = 1;
					m_bGetItem = true;
					--m_vecAIStuff[k].count;
					check = true;
					break;
				}
			}

			if (!check)
			{
				tItem item = CItemManager::GetInst()->GetItemInfo(_target->m_arrEquipItems[j]);
				map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(item.ModelName);

				if (m_mapAIItemLoot.end() != iter)
				{
					m_arrInventory[i].ID = _target->m_arrEquipItems[j];
					m_arrInventory[i].count = 1;
					m_bGetItem = true;
					iter = m_mapAIItemLoot.erase(iter);
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	// 필요한 거 다 먹은 거에 대한 체크가 있어야됨

	// 빈 거 체크
	bool checkInven = true;
	bool checkEquip = true;

	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END != _target->m_arrInventory[i].ID)
		{
			checkInven = false;
			break;
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		if (ITEM::END != _target->m_arrEquipItems[i])
		{
			checkEquip = false;
			break;
		}
	}

	if (checkEquip && checkInven)
	{
		_target->m_bAICheck = false;
	}

	m_aiLootTarget = nullptr;
}

void CPlayerScript::AILooting(CMonsterScript* _target)
{
	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END != m_arrInventory[i].ID)
			continue;

		for (int j = 0; j < 5; ++j)
		{
			ITEM& item = _target->GetBoxItem(j);

			if (ITEM::END == item)
				continue;

			bool check = false;
			for (size_t k = 0; k < m_vecAIStuff.size(); ++k)
			{
				if (0 == m_vecAIStuff[k].count)
					continue;

				if (item == m_vecAIStuff[k].item)
				{
					m_arrInventory[i].ID = item;
					m_arrInventory[i].count = 1;
					m_bGetItem = true;
					--m_vecAIStuff[k].count;
					check = true;
					item = ITEM::END;
					break;
				}
			}

			if (!check)
			{
				tItem itemInfo = CItemManager::GetInst()->GetItemInfo(item);
				map<wstring, ITEM>::iterator iter = m_mapAIItemLoot.find(itemInfo.ModelName);

				if (m_mapAIItemLoot.end() != iter)
				{
					m_arrInventory[i].ID = item;
					m_arrInventory[i].count = 1;
					m_bGetItem = true;
					iter = m_mapAIItemLoot.erase(iter);
					item = ITEM::END;
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	m_bGetItem = true;

	// 필요한 거 다 먹은 거에 대한 체크가 있어야됨

	// 이건 빈거 체크
	bool checkBox = true;

	for (int i = 0; i < 5; ++i)
	{
		ITEM& item = _target->GetBoxItem(i);

		if (ITEM::END != item)
		{
			checkBox = false;
			break;
		}
	}

	if (checkBox)
	{
		_target->SetCheck(false);
	}

	m_aiLootTarget = nullptr;
}

void CPlayerScript::CheckInvenEmpty()
{
	bool check = false;

	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END == m_arrInventory[i].ID)
		{
			check = true;
			break;
		}
	}

	if (check)
		m_bBoxEmpty = true;
	else
		m_bBoxEmpty = false;
}

void CPlayerScript::AIEquipChange(EQUIP_TYPE _type)
{
	m_bEquipChange = true;
	tItem& equipItem = CItemManager::GetInst()->GetItemInfo(m_arrEquipItems[(UINT)_type]);

	for (int i = 0; i < 10; ++i)
	{
		tItem& item = CItemManager::GetInst()->GetItemInfo(m_arrInventory[i].ID);

		if (EQUIP_TYPE::WEAPON == _type)
		{
			for (size_t j = 0; j < m_vecWeaponType.size(); ++j)
			{
				if (m_vecWeaponType[j] == item.Type && equipItem.Grade < item.Grade)
				{
					ITEM temp = m_arrEquipItems[(UINT)_type];
					m_arrEquipItems[(UINT)_type] = m_arrInventory[i].ID;
					m_arrInventory[i].ID = temp;
					return;
				}
			}
		}
		else if (EQUIP_TYPE::HEAD == _type)
		{
			if (ITEM_TYPE::HEAD == item.Type && equipItem.Grade < item.Grade)
			{
				ITEM temp = m_arrEquipItems[(UINT)_type];
				m_arrEquipItems[(UINT)_type] = m_arrInventory[i].ID;
				m_arrInventory[i].ID = temp;
				return;
			}
		}
		else if (EQUIP_TYPE::CHEST == _type)
		{
			if (ITEM_TYPE::CHEST == item.Type && equipItem.Grade < item.Grade)
			{
				ITEM temp = m_arrEquipItems[(UINT)_type];
				m_arrEquipItems[(UINT)_type] = m_arrInventory[i].ID;
				m_arrInventory[i].ID = temp;
				return;
			}
		}
		else if (EQUIP_TYPE::ARM == _type)
		{
			if (ITEM_TYPE::ARM == item.Type && equipItem.Grade < item.Grade)
			{
				ITEM temp = m_arrEquipItems[(UINT)_type];
				m_arrEquipItems[(UINT)_type] = m_arrInventory[i].ID;
				m_arrInventory[i].ID = temp;
				return;
			}
		}
		else if (EQUIP_TYPE::LEG == _type)
		{
			if (ITEM_TYPE::LEG == item.Type && equipItem.Grade < item.Grade)
			{
				ITEM temp = m_arrEquipItems[(UINT)_type];
				m_arrEquipItems[(UINT)_type] = m_arrInventory[i].ID;
				m_arrInventory[i].ID = temp;
				return;
			}
		}
		else if (EQUIP_TYPE::ACCESSORY == _type)
		{
			if (ITEM_TYPE::ACCESSORY == item.Type && equipItem.Grade < item.Grade)
			{
				ITEM temp = m_arrEquipItems[(UINT)_type];
				m_arrEquipItems[(UINT)_type] = m_arrInventory[i].ID;
				m_arrInventory[i].ID = temp;
				return;
			}
		}
	}
}

void CPlayerScript::CharacterDead()
{
	SetAnimAction(PLAYER_ANIMTYPE::COMMON_ANIM, (UINT)PLAYER_COMMONANIM::COMMON_DEATH, false);
	
	if (nullptr != m_dieSound && IsPlaySound())
	{
		switch (m_characterType)
		{
		case CHARACTER_TYPE::AYA:
			CResourceManager::GetInst()->FindRes<CSound>(L"Aya_Die").Get()->Play(1, 0.1f);
			break;
		case CHARACTER_TYPE::HYUNWOO:
			CResourceManager::GetInst()->FindRes<CSound>(L"Hyunwoo_Die").Get()->Play(1, 0.1f);
			break;
		case CHARACTER_TYPE::LIDAILIN:
			CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_Die").Get()->Play(1, 0.1f);
			break;
		default:
			break;
		}
	
	}

	m_bAIMove = false;
	m_bMoveMotion = false;
	m_bAttack = false;

	m_trackingUI->SetObjOff();
}

float CPlayerScript::GetSkillCTRatio(SKILL_TYPE _type)
{
	float ratio = 0.f;
	switch (_type)
	{
	case SKILL_TYPE::Q:
	{
		if (m_skill_Q_MaxCT > 0.f)
			ratio = m_skill_Q_CT / m_skill_Q_MaxCT;
		else
			ratio = 1.f;
	}
	break;
	case SKILL_TYPE::W:
	{
		if (m_skill_W_MaxCT > 0.f)
			ratio = m_skill_W_CT / m_skill_W_MaxCT;
		else
			ratio = 1.f;
	}
	break;
	case SKILL_TYPE::E:
	{
		if (m_skill_E_MaxCT > 0.f)
			ratio = m_skill_E_CT / m_skill_E_MaxCT;
		else
			ratio = 1.f;
	}
	break;
	case SKILL_TYPE::R:
	{
		if (m_skill_R_MaxCT > 0.f)
			ratio = m_skill_R_CT / m_skill_R_MaxCT;
		else
			ratio = 1.f;
	}
	break;
	}

	return ratio;
}

bool CPlayerScript::CanSkillLevelUp(SKILL_TYPE _type)
{
	bool bLevelUp = false;

	switch (_type)
	{
	case SKILL_TYPE::Q:
	{
		if (m_skill_Q_Lv < m_skill_Q_MaxLv)
			bLevelUp = true;
		else
			bLevelUp = false;
	}
	break;
	case SKILL_TYPE::W:
	{
		if (m_skill_W_Lv < m_skill_W_MaxLv)
			bLevelUp = true;
		else
			bLevelUp = false;
	}
	break;
	case SKILL_TYPE::E:
	{
		if (m_skill_E_Lv < m_skill_E_MaxLv)
			bLevelUp = true;
		else
			bLevelUp = false;
	}
	break;
	case SKILL_TYPE::R:
	{
		if (m_skill_R_Lv < m_skill_R_MaxLv)
			bLevelUp = true;
		else
			bLevelUp = false;
	}
	break;
	case SKILL_TYPE::T:
	{
		if (m_passive_LV < m_passive_MaxLV)
			bLevelUp = true;
		else
			bLevelUp = false;
	}
	break;
	}

	return bLevelUp;
}

int CPlayerScript::GetSkillLevel(SKILL_TYPE _type)
{
	switch (_type)
	{
	case SKILL_TYPE::Q:
		return m_skill_Q_Lv;
	case SKILL_TYPE::W:
		return m_skill_W_Lv;
	case SKILL_TYPE::E:
		return m_skill_E_Lv;
	case SKILL_TYPE::R:
		return m_skill_R_Lv;
	case SKILL_TYPE::T:
		return m_passive_LV;
	default:
		return -1;
	}
}

bool CPlayerScript::IsSkillUsed(SKILL_TYPE _type)
{
	switch (_type)
	{
	case SKILL_TYPE::Q:
		return m_bSkill_Q_Motion;
	case SKILL_TYPE::W:
		return m_bSkill_W_Motion;
	case SKILL_TYPE::E:
		return m_bSkill_E_Motion;
	case SKILL_TYPE::R:
		return m_bSkill_R_Motion;
	}

	return false;
}

ITEM& CPlayerScript::GetBoxItem(int _index)
{
	if (_index < 0 || _index > 15)
	{
		ITEM item = ITEM::END;
		return item;
	}

	if (m_vecBox.empty())
	{
		for (int i = 0; i < 10; ++i)
		{
			if (ITEM::END == m_arrInventory[i].ID)
				continue;

			m_vecBox.push_back(m_arrInventory[i].ID);
		}

		for (int i = 0; i < 6; ++i)
		{
			if (ITEM::END == m_arrEquipItems[i])
				continue;

			m_vecBox.push_back(m_arrEquipItems[i]);
		}
	}

	if (_index >= m_vecBox.size())
	{
		ITEM item = ITEM::END;
		return item;
	}

	return m_vecBox[_index];
}

void CPlayerScript::LootEnd(int _index)
{
	if (_index < 0 || _index > 15 || _index >= m_vecBox.size())
		return;

	m_vecBox[_index] = ITEM::END;
}

void CPlayerScript::ChangeEquipNInventoryItem(UINT _equipIndex, UINT _InventoryIndex)
{
	ITEM equip = m_arrEquipItems[_equipIndex];
	ITEM item = m_arrInventory[_InventoryIndex].ID;

	m_arrEquipItems[_equipIndex] = item;
	m_arrInventory[_InventoryIndex].ID = equip;
}

void CPlayerScript::ReleaseEquip(UINT _equipIndex)
{
	int index = 0;

	if (_equipIndex < 19)
		index = 0;
	else
		index = _equipIndex - 18;

	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END == m_arrInventory[i].ID)
		{
			m_arrInventory[i].ID = m_arrEquipItems[index];
			m_arrInventory[i].count = 1;
			m_arrEquipItems[index] = ITEM::END;
			break;
		}
	}
}

void CPlayerScript::LootItem(ITEM& _item)
{
	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END == m_arrInventory[i].ID)
		{
			m_arrInventory[i].ID = _item;
			m_arrInventory[i].count = 1;
			_item = ITEM::END;
			break;
		}
	}
}

void CPlayerScript::Attack()
{
	// 타깃 지정시 타깃의 거리가 공격 범위 내라면 데미지를 준다.
	if (nullptr == m_attackTarget)
		return;

	if (((CObjectScript*)m_attackTarget->GetScripts()[0])->IsCharacterDead() && m_attackTarget->GetObjectType() == OBJECT_TYPE::MONSTER)
	{
		return;
	}

	if (PLAYER_TYPE::PLAYER == m_playerType && !m_bGoToAttack)
	{
		return;
	}

	Vec3 targetPos = m_attackTarget->Transform()->GetWorldPos_XZ();
	Vec3 pos = Transform()->GetWorldPos_XZ();
	float dist = Vec3::Distance(pos, targetPos);

	if (nullptr != m_attackTarget && !m_bTargetAttack && m_bAttack)
	{
		//어택 하는 중일때 예외처리 체크1
		// 애니메이션 이 특정 프레임 일 때만 공격 해야됨
		// 현우 atk1 일때 10 , atk2는 4 프레임
		// 리다이린 5 , 6

		if (m_characterType == CHARACTER_TYPE::HYUNWOO && !m_bTargetAttack)
		{
			if (!m_bFristAttackMotion && Animator3D()->GetCurFrameIndex() == 10 && m_curAnim == (UINT)HYUNWOO_ANIM::ATK01)
			{
				m_bTargetAttack = true;
				TargetAttack();
			}
			else if (m_bFristAttackMotion && Animator3D()->GetCurFrameIndex() == 4 && m_curAnim == (UINT)HYUNWOO_ANIM::ATK02)
			{
				m_bTargetAttack = true;
				TargetAttack();
			}
		}
		else if (m_characterType == CHARACTER_TYPE::LIDAILIN && !m_bTargetAttack)
		{
			if (!m_bFristAttackMotion && Animator3D()->GetCurFrameIndex() == 5 && m_curAnim == (UINT)LIDAILIN_ANIM::ATK01)
			{
				m_bTargetAttack = true;
				TargetAttack();
			}
			else if (m_bFristAttackMotion && Animator3D()->GetCurFrameIndex() == 6 && m_curAnim == (UINT)LIDAILIN_ANIM::ATK02)
			{
				m_bTargetAttack = true;
				TargetAttack();
			}
		}

		if (nullptr != m_attackSound.Get() && IsPlaySound())
			m_attackSound->Play(1, 0.1f);
	}

	Vec3 rot = Transform()->GetLocalRot();
	Vec3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	front.Normalize();
	Vec3 dir = (targetPos - pos).Normalize();

	if (m_bAttack)
	{
		m_rotAngle = 0.f;

		Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
		Vec3 crossVec = front.Cross(dir);
		float sign = crossVec.Dot(up);

		if (sign > 0.f)
		{
			m_rotAngle += acosf(front.Dot(dir)) - XM_PI;
		}
		else if (sign < 0.f)
		{
			m_rotAngle -= acosf(front.Dot(dir)) - XM_PI;
		}

		float yAngle = rot.y;

		rot.y += m_rotAngle;

		if (_isnanf(rot.y))
			rot.y = yAngle;

		Transform()->SetLocalRot(rot);
	}

	if (dist < m_attackRange && !m_bMotion)	//스킬 사용중 아닐대, 어택 모션일때 
	{
		m_bAttack = true;
		m_bAttackDistance = true;
	}
	else if (dist > m_attackRange && !m_naviMesh->NaviMesh()->IsMove())
	{
		m_bAttackDistance = false;
	}

}

void CPlayerScript::Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner)
{
	// 맞았는지 확인 및 체력 깎음
	m_bHit = true;

	if(PLAYER_TYPE::AI == m_playerType && nullptr != _owner && _owner->GetObjectType() == OBJECT_TYPE::PLAYER)
		m_attackTarget = _owner;

	float def = m_finalDefense * (1 - m_defReduce);
	float baseDamage = _damage * 100 / (100 + def);
	float extraDamage_percent;
	float extraDamage_flat;
	float finalBaseDamage = (baseDamage * _critical);
	float finalDamage = 0.f;

	// 계산 다시 해야됨
	if (ATTACK_TYPE::BASE == _type)
	{
		extraDamage_percent = 1 + _extraPercent;
		extraDamage_flat = _extraFlat - m_DefenseFromBasicAttack;
	}
	else
	{
		extraDamage_percent = 1 + _extraPercent - m_SkillDamageReduction_Percent;
		extraDamage_flat = _extraFlat - m_SkillDamageReduction;
	}

	if (1.f < _critical)
	{
		finalBaseDamage *= (1 - m_CriticalDamageReduction);
	}

	finalDamage = finalBaseDamage * extraDamage_percent + extraDamage_flat;

	if (finalDamage < 1.f)
	{
		finalDamage = 1.f;
	}

	m_curHP -= finalDamage;

	if (nullptr != m_hitEffect)
	{
		CGameObject* effect = m_hitEffect->Instantiate();
		((CEffectScript*)effect->GetScripts()[0])->SetAnimMode();
		((CEffectScript*)effect->GetScripts()[0])->SetTargetObj(Object());
		((CEffectScript*)effect->GetScripts()[0])->SetEffectColor(Vec3(0.886f, 0.886f, 0.886f));
		Vec2 scale = Object()->Transform()->GetLocalScale().XY();
		effect->Transform()->SetLocalScale(Vec3(scale.x * 1.5f, scale.y * 1.5f, 1.f));
		CScript::CreateObject(effect, 29);
	}

	CObjectScript::Hit(_type, _damage, _critical, _extraPercent, _extraFlat, _owner);

	if (m_bCharacterDead && nullptr != _owner && _owner->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		((CPlayerScript*)_owner->GetScripts()[0])->m_curExp += 300.f * m_level;
	}
}

void CPlayerScript::TargetAttack()
{
	// 충돌한 물체가 몬스터이거나 Ai인지 확인
	if (OBJECT_TYPE::MONSTER == m_attackTarget->GetObjectType() || (m_attackTarget != Object() && OBJECT_TYPE::PLAYER == m_attackTarget->GetObjectType()))
	{
		srand((unsigned int)time(NULL));
		int critical = rand() % 100;
		float extraDamage = m_ExtraBaseAttackDamage + (m_ExtraBaseAttackDamagePerLevel * m_level);

		// 충돌한 물체에게 데미지 부여
		if (OBJECT_TYPE::MONSTER == m_attackTarget->GetObjectType())
		{
			if (critical < int(m_finalCriticalChance * 100.f))
			{
				((CMonsterScript*)m_attackTarget->GetScripts()[0])->Hit(ATTACK_TYPE::BASE, m_finalAttackPower, m_finalCriticalDamage, 0, extraDamage, Object());
			}
			else
			{
				((CMonsterScript*)m_attackTarget->GetScripts()[0])->Hit(ATTACK_TYPE::BASE, m_finalAttackPower, 1.f, 0, extraDamage, Object());
			}
		}
		else if (m_attackTarget != Object() && OBJECT_TYPE::PLAYER == m_attackTarget->GetObjectType())
		{
			if (critical < int(m_finalCriticalChance * 100.f))
			{
				((CPlayerScript*)m_attackTarget->GetScripts()[0])->Hit(ATTACK_TYPE::BASE, m_finalAttackPower, m_finalCriticalDamage, 0, extraDamage, Object());
			}
			else
			{
				((CPlayerScript*)m_attackTarget->GetScripts()[0])->Hit(ATTACK_TYPE::BASE, m_finalAttackPower, 1.f, 0, extraDamage, Object());
			}
		}
	}
}

void CPlayerScript::OnColliderEnter(CCollider3D* _other)
{
	if (_other->Object()->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::LAYERUPDATEBOX)
	{
		int layerNamber = ((CLayerUpdateBox*)_other->Object()->GetScripts()[0])->GetLayerNumber();
		CSceneManager::GetInst()->GetCurScene()->GetLayer(layerNamber)->SetLayerAction(true);
	}

	if (m_bCharacterDead)
	{
		// 충돌 시 아이템 UI 오픈
		g_itemBox->SetOnOff(true);
	}
}

void CPlayerScript::OnCollider(CCollider3D* _other)
{
	if (_other->Object()->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::LAYERUPDATEBOX)
	{
		//레이어 박스 배치 되고 난 후
		int layerNamber = ((CLayerUpdateBox*)_other->Object()->GetScripts()[0])->GetLayerNumber();
		const vector<CGameObject*>& vecAllObj = CSceneManager::GetInst()->GetCurScene()->GetLayer(layerNamber)->GetAllObj();
		Vec3 playerPos = Transform()->GetWorldPos().XZ();
		float radius = m_visionRange / 2.f;

		for (size_t i = 0; i < vecAllObj.size(); i++)
		{
			if (vecAllObj[i]->MeshRender() && nullptr == vecAllObj[i]->MeshRender()->GetMesh().Get() || vecAllObj[i]->GetUpdateAction())
				continue;

			Vec3 objPos = vecAllObj[i]->Transform()->GetWorldPos().XZ();
			float Length = Vec3::Distance(playerPos, objPos);

			if (3000.f > Length)
			{
				vecAllObj[i]->SetUpdateAction();
			}
		}
	}

}

void CPlayerScript::OnColliderExit(CCollider3D* _other)
{
	if (_other->Object()->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::LAYERUPDATEBOX)
	{
		int layerNamber = ((CLayerUpdateBox*)_other->Object()->GetScripts()[0])->GetLayerNumber();
		CSceneManager::GetInst()->GetCurScene()->GetLayer(layerNamber)->SetLayerAction(false);
	}

	if (m_bCharacterDead)
	{
		// 아이템 UI 닫기
		g_itemBox->SetOnOff(false);
	}
}

void CPlayerScript::StatsReSet()
{
	m_skill_Q_Lv = 0;
	m_skill_W_Lv = 0;
	m_skill_E_Lv = 0;
	m_skill_R_Lv = 0;
	m_level = 0;
	m_skillPoint = 0;
	LvUp();

}

void CPlayerScript::SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop, float _animSpeed, UINT _count)
{
	wstring animName = L"Common_";

	switch ((PLAYER_COMMONANIM)_AnimType)
	{
	case PLAYER_COMMONANIM::COMMON_DEATH:
		if (CHARACTER_TYPE::HYUNWOO ==  m_characterType)
			animName += L"Death";
		else
			animName += L"death";
		break;
	case PLAYER_COMMONANIM::COMMON_RUN:
		animName += L"Run";
		break;
	case PLAYER_COMMONANIM::COMMON_WAIT:
		animName += L"wait";
		break;
	case PLAYER_COMMONANIM::COMMON_REST_END:
		animName += L"rest_End";
		break;
	case PLAYER_COMMONANIM::COMMON_REST_LOOP:
		animName += L"rest_Loop";
		break;
	case PLAYER_COMMONANIM::COMMON_REST_START:
		animName += L"rest_Start";
		break;
	case PLAYER_COMMONANIM::COMMON_COLLECT:
		animName += L"collect";
		break;
	case PLAYER_COMMONANIM::COMMON_BOXOPEN:
		animName += L"boxopen";
		break;
	case PLAYER_COMMONANIM::COMMON_FISHING:
		animName += L"fishing";
		break;
	case PLAYER_COMMONANIM::COMMON_CRAFTMETAL:
		animName += L"craftmetal";
		break;
	case PLAYER_COMMONANIM::COMMON_CRAFTFOOD:
		animName += L"craftfood";
		break;
	case PLAYER_COMMONANIM::COMMON_OPERATE:
		animName += L"operate";
		break;
	case PLAYER_COMMONANIM::COMMON_ARRIVE:
		animName += L"arrive";
		break;
	case PLAYER_COMMONANIM::COMMON_INSTALL_TRAP:
		animName += L"install_trap";
		break;
	case PLAYER_COMMONANIM::COMMON_COLLECT_WATER:
		animName += L"collect_Water";
		break;
	case PLAYER_COMMONANIM::COMMON_COLLECT_WOOD:
		animName += L"collect_Wood";
		break;
	case PLAYER_COMMONANIM::COMMON_COLLECT_STONE:
		animName += L"collect_Stone";
		break;
	case PLAYER_COMMONANIM::COMMON_DANCE:
		animName += L"Dance";
		break;
	case PLAYER_COMMONANIM::COMMON_RESURRECT:
		animName += L"resurrect";
		break;
	case PLAYER_COMMONANIM::COMMON_DOWN_WAIT:
		animName += L"down_wait";
		break;
	case PLAYER_COMMONANIM::COMMON_DOWN_RUN:
		animName += L"down_run";
		break;
	case PLAYER_COMMONANIM::COMMON_DOWN_DEAD:
		animName += L"down_dead";
		break;
	case PLAYER_COMMONANIM::COMMON_DOWN_START:
		animName += L"down_start";
		break;
	case PLAYER_COMMONANIM::END:
		return;
	default:
		return;
	}

	Animator3D()->Play(animName, _bLoop, _animSpeed, _count);
	m_animType = _action;
	m_curAnim = _AnimType;
}

void CPlayerScript::SaveToScene(FILE* _file)
{
	CObjectScript::SaveToScene(_file);

	SaveToFile(&m_playerType, _file);
}

void CPlayerScript::LoadFromScene(FILE* _file)
{
	CObjectScript::LoadFromScene(_file);

	LoadFromFile(&m_playerType, _file);
}
