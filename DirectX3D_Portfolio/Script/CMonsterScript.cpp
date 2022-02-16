#include "pch.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CMonsterManager.h"
#include "CEffectScript.h"
#include "CHitBoxScript.h"
#include "CItemManager.h"
#include "CUIScript.h"

#include <Engine\CLayer.h>
#include <Engine\CScene.h>
#include <Engine\CAnimator3D.h>
#include <Engine/CSceneManager.h>
#include <Engine/CNaviMesh.h>


CMonsterScript::CMonsterScript() :
	CObjectScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
	m_appear(APPEAR::CHICKEN),
	m_arrBox{},
	m_startPos(Vec3(0.f, 0.f, 0.f)) ,
	m_startDir(),
	m_traceDistance(600.f),
	m_bSKillMotion(false),
	m_specialAttack(0.f),
	m_monsterState(MONSTER_STATE::END),
	m_monsterType(MONSTER_TYPE::END),
	m_bDeadAnim(false),
	m_bHpRecovery(false),
	m_curAssaultDistance(0.f),
	m_AttackCooldownTime(0.f),
	m_SKillCooldownTime(0.f),
	m_assaultDistance(600.f),
	m_bAssaultMotion(false),
	m_bReadyMotion(false),
	m_bSKillExistence(false),
	m_bSkillCooldownOn(true),
	m_hpRecoveryCooldownTime(0.f),
	m_monsterAnimAction(MONSTER_ANIMCLIP::END),
	m_bEndBattleMotion(false),
	m_bRevert(false),
	m_bMonsterMove(true),
	m_bMonsterRenew(true),
	m_skillEffect_01(nullptr),
	m_skillEffect_02(nullptr)
{
	SetName(L"MonsterScript");

	m_maxStamina = 100.f;
	m_curStamina = m_maxStamina;
}

CMonsterScript::CMonsterScript(const CMonsterScript& _origin) :
	CObjectScript(_origin),
	m_appear(_origin.m_appear),
	m_arrBox{},
	m_startPos(),
	m_startDir(),
	m_traceDistance(600.f),
	m_bSKillMotion(false),
	m_specialAttack(0.f),
	m_monsterState(MONSTER_STATE::END),
	m_monsterType(_origin.m_monsterType),
	m_bDeadAnim(false),
	m_bHpRecovery(false),
	m_curAssaultDistance(0.f),
	m_AttackCooldownTime(0.f),
	m_SKillCooldownTime(0.f),
	m_assaultDistance(600.f),
	m_bAssaultMotion(false),
	m_bReadyMotion(false),
	m_bSKillExistence(false),
	m_bSkillCooldownOn(true),
	m_bMonsterMove(true),
	m_bMonsterRenew(true),
	m_hpRecoveryCooldownTime(0.f),
	m_monsterAnimAction(MONSTER_ANIMCLIP::END),
	m_bEndBattleMotion(false),
	m_bRevert(false),
	m_skillEffect_01(nullptr),
	m_skillEffect_02(nullptr)
{
	SetName(L"MonsterScript");

	m_maxStamina = 100.f;
	m_curStamina = m_maxStamina;
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::init()
{
	// 몬스터 스탯의 경우 몬스터 매니저를 만들었을 때 몬스터 매니저가 지급하게 제작할 것이기 때문에
	// 몬스터 매니저가 추가 되면 넣을 예정
	// 또한 몬스터의 시작 위치를 이때 넣어줄 예정

	//boar나 bear의 attack 박스를 만들어줘야 하는데, 스케일 지정및 매쉬와 메테리얼도 넣어줘야함
	// 그럼 콜라이더 체크는??
}


void CMonsterScript::start()
{
	CObjectScript::start();
	Object()->SetObjectType(OBJECT_TYPE::MONSTER);
	m_monsterState = MONSTER_STATE::MONSTER_IDLE;

	if (nullptr == m_trackingUI)
	{
		Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"MonsterTrackingUI", L"prefab\\RootMonsterTrackingUI.prefab");
		m_trackingUI = prefab->Instantiate();
		for (size_t i = 0; i < m_trackingUI->GetChilds().size(); ++i)
		{
			if (!m_trackingUI->GetChilds()[i]->GetScripts().empty())
				((CUIScript*)m_trackingUI->GetChilds()[i]->GetScripts()[0])->SetTarget(Object());
		}
		((CUIScript*)m_trackingUI->GetScripts()[0])->SetTarget(Object());
		CScript::CreateObject(m_trackingUI, Vec3(0.f, 0.f, 0.f), 29);
	}

	SetMonsterType(m_monsterType);
	MonsterItem();

	if(ITEM::END == m_arrBox[0])
		m_arrBox[0] = ITEM::MEAT;

	if (m_startPos.Length() == 0.f)
	{
		m_startPos = Object()->Transform()->GetLocalPos();
	}
}

void CMonsterScript::update()
{
	CObjectScript::update();

	if (m_bCharacterDead && 0.f > m_curHP && MONSTER_STATE::MONSTER_DEAD != m_monsterState)
	{
		m_monsterState = MONSTER_STATE::MONSTER_DEAD;
	}

	if (!m_bCharacterDead && nullptr != m_attackBox && m_bSKillExistence && !m_bSKillMotion && m_attackBox->IsOn())
	{
		m_attackBox->SetOnOff(false);
		((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuff(DEBUFF_TYPE::ALL, false);
		((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetRate(false);
	}

	switch (m_monsterState)
	{
	case MONSTER_STATE::MONSTER_IDLE:
		IdleState();
		break;
	case MONSTER_STATE::MONSTER_TRACE:
		TraceState();
		break;
	case MONSTER_STATE::MONSTER_ATTACK:
		AttackState();
		break;
	case MONSTER_STATE::MONSTER_REVERT:
		RevertState();
		break;
	case MONSTER_STATE::MONSTER_DEAD:
		DeadState();
		return;
		break;
	case MONSTER_STATE::END:
		break;
	default:
		break;
	}

	if (!m_bSkillCooldownOn)
	{
		m_SKillCooldownTime += DELTATIME;

		switch (m_monsterType)
		{
		case MONSTER_TYPE::MONSTER_BEAR:
			if (m_SKillCooldownTime > 5.f)
			{
				m_bSkillCooldownOn = true;
			}
			break;
		case MONSTER_TYPE::MONSTER_BOAR:
			if (m_SKillCooldownTime > 5.f)
			{
				m_bSkillCooldownOn = true;
			}
			break;
		case MONSTER_TYPE::MONSTER_WOLF:
			if (m_SKillCooldownTime > 20.f)
			{
				m_bSkillCooldownOn = true;
			}
			break;
		default:
			return;
		}
	}
}

void CMonsterScript::lateUpdate()
{
	if (m_bCharacterDead)
		return;

	Vec3 pos = Transform()->GetLocalPos();

	if (_isnanf(pos.x) || _isnanf(pos.y) || _isnanf(pos.z) || 0.f == pos.x || 0.f == pos.z)
	{
		Transform()->SetLocalPos(m_startPos);
		Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	}

	if (SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode())
	{
		if (m_curHP <= 0)
		{
			m_monsterState = MONSTER_STATE::MONSTER_DEAD;
		}

		if (nullptr == m_naviMesh)
		{
			m_naviMesh = CSceneManager::GetInst()->FindObjByName(L"NaviMesh");
		}

		if (nullptr != m_naviMesh && m_naviMesh->NaviMesh()->IsAboveTile(Object(), Vec3(0.f, 0.f, 0.f)))
		{
			m_curNode = m_naviMesh->NaviMesh()->GetCurNode(Object());

			if (nullptr != m_curNode)
			{
				m_curNode->SetMove(false);
			}
		}
	}
}

void CMonsterScript::SetMonsterType(MONSTER_TYPE _type)
{
	switch (_type)
	{
	case MONSTER_TYPE::MONSTER_BAT:
	{
		m_level = 1;
		m_maxHP = 360.f;
		m_curHP = m_maxHP;
		m_offense = 38.f;
		m_defense = 24.f;
		m_attackSpeed = 0.6f;
		m_moveSpeed = 5.f;
		m_appear = APPEAR::BAT;

		m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"batAttack", L"sound\\bat\\batAttack.wav");
		m_hitSound = CResourceManager::GetInst()->Load<CSound>(L"batHit", L"sound\\bat\\batHit.wav");
		m_dieSound = CResourceManager::GetInst()->Load<CSound>(L"batDie", L"sound\\bat\\batDie.wav");
		m_wakeUpSound = CResourceManager::GetInst()->Load<CSound>(L"batWakeupStart", L"sound\\bat\\batWakeUp_Start.wav");
	}
	break;
	case MONSTER_TYPE::MONSTER_BEAR:
	{
		m_level = 6;
		m_maxHP = 1220.f;
		m_curHP = m_maxHP;
		m_offense = 175.f;
		m_defense = 63.f;
		m_attackSpeed = 0.6f;
		m_moveSpeed = 3.05f;
		m_specialAttack = m_offense * 2.f;
		m_bSKillExistence = true;
		m_appear = APPEAR::BEAR;

		vector<CGameObject*> childObj = Object()->GetChilds();

		if (!childObj.empty())
		{
			for (int i = 0; i < childObj.size(); ++i)
			{
				if (L"Bear_Skill_Mesh" == childObj[i]->GetName())
				{
					m_skillEffect_01 = childObj[i];
					CMaterial* material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
					material = material->Clone();
					m_skillEffect_01->MeshRender()->SetMaterial(material);

					Vec3 color = Vec3(1.f, 0.1f, 0.1f);
					material->SetData(SHADER_PARAM::VEC4_0, &color);
					break;
				}
			}
		}

		if (nullptr == m_skillEffect_01)
		{
			Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"Bear_Skill_Mesh", L"prefab\\Bear_Skill_Mesh.prefab");
			m_skillEffect_01 = prefab->Instantiate();
			m_skillEffect_01->SetName(L"Bear_Skill_Mesh");
			m_skillEffect_01->Transform()->SetLocalPos(Vec3(-3.85f, 0.f, -1.f));
			m_skillEffect_01->Transform()->SetLocalScale(Vec3(0.015f, 0.015f, 0.015f));
			CMaterial* material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
			material = material->Clone();
			m_skillEffect_01->MeshRender()->SetMaterial(material);

			Vec4 color = Vec4(1.f, 0.1f, 0.1f, 1.f);
			material->SetData(SHADER_PARAM::VEC4_0, &color);

			CScript::AddChild(Object(), m_skillEffect_01);
		}

		m_skillEffect_01->SetObjOff();

		m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"bearAttack", L"sound\\bear\\bearAttack.wav");
		m_hitSound = CResourceManager::GetInst()->Load<CSound>(L"bearHit", L"sound\\bear\\bearHit.wav");
		m_dieSound = CResourceManager::GetInst()->Load<CSound>(L"bearDie", L"sound\\bear\\bearDie.wav");
		m_wakeUpSound = CResourceManager::GetInst()->Load<CSound>(L"bearWakeupStart", L"sound\\bear\\bearWakeUp_Start_v1.wav");
		m_skillActiveSound = CResourceManager::GetInst()->Load<CSound>(L"bearSkillActivation", L"sound\\bear\\bear_Skill_Activation.wav");
		m_skillImpactSound = CResourceManager::GetInst()->Load<CSound>(L"bearSkillImpact", L"sound\\bear\\bear_Skill_Impact.wav");
		m_skillHitSound = CResourceManager::GetInst()->Load<CSound>(L"bearSkillHit", L"sound\\bear\\bear_Skill_Hit.wav");

	}
	break;
	case MONSTER_TYPE::MONSTER_BOAR:
	{
		m_level = 2;
		m_maxHP = 942.f;
		m_curHP = m_maxHP;
		m_offense = 51.f;
		m_defense = 16.f;
		m_attackSpeed = 0.5f;
		m_moveSpeed = 3.03f;
		m_specialAttack = m_offense * 1.5f;
		m_bSKillExistence = true;
		m_appear = APPEAR::BOAR;

		vector<CGameObject*> childObj = Object()->GetChilds();

		if (!childObj.empty())
		{
			for (int i = 0; i < childObj.size(); ++i)
			{
				if (L"Boar_Skill01_Mesh" == childObj[i]->GetName())
				{
					m_skillEffect_01 = childObj[i];
					CMaterial* material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
					material = material->Clone();
					m_skillEffect_01->MeshRender()->SetMaterial(material);

					Vec3 color = Vec3(1.f, 0.1f, 0.1f);
					material->SetData(SHADER_PARAM::VEC4_0, &color);
				}
				if (L"FX_BI_Boar_Skill01_HitRange02" == childObj[i]->GetName())
				{
					m_skillEffect_02 = childObj[i];

					CMaterial* material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
					material = material->Clone();
					m_skillEffect_02->MeshRender()->SetMaterial(material);

					Vec3 color = Vec3(1.f, 0.1f, 0.1f);
					material->SetData(SHADER_PARAM::VEC4_0, &color);
				}
			}
		}

		if (nullptr == m_skillEffect_01)
		{
			Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"Boar_Skill01_Mesh", L"prefab\\Boar_Skill01_Mesh.prefab");
			m_skillEffect_01 = prefab->Instantiate();
			m_skillEffect_01->SetName(L"Boar_Skill01_Mesh");
			m_skillEffect_01->Transform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.08f));

			CMaterial* material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
			material = material->Clone();
			m_skillEffect_01->MeshRender()->SetMaterial(material);

			Vec4 color = Vec4(1.f, 0.1f, 0.1f, 1.f);
			material->SetData(SHADER_PARAM::VEC4_0, &color);

			CScript::AddChild(Object(), m_skillEffect_01);
		}
		
		if (nullptr == m_skillEffect_02)
		{
			Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"Boar_Skill02_Mesh", L"prefab\\Boar_Skill02_Mesh.prefab");
			m_skillEffect_02 = prefab->Instantiate();
			m_skillEffect_02->SetName(L"Boar_Skill02_Mesh");
			m_skillEffect_02->Transform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.08f));
			CMaterial* material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
			material = material->Clone();
			m_skillEffect_02->MeshRender()->SetMaterial(material);

			Vec3 color = Vec3(1.f, 0.1f, 0.1f);
			material->SetData(SHADER_PARAM::VEC4_0, &color);

			CScript::AddChild(Object(), m_skillEffect_02);
		}

		m_skillEffect_01->SetObjOff();
		m_skillEffect_02->SetObjOff();

		if (nullptr != m_attackBox)
		{
			m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, -90.f));
			m_attackBox->Collider3D()->SetOffsetScale(Vec3(1.15f, 1.f, 1.f));
		}

		m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"boarAttack", L"sound\\boar\\boarAttack.wav");
		m_hitSound = CResourceManager::GetInst()->Load<CSound>(L"boarHit", L"sound\\boar\\boarHit.wav");
		m_dieSound = CResourceManager::GetInst()->Load<CSound>(L"boarDie", L"sound\\boar\\boarDie.wav");
		m_skillActiveSound = CResourceManager::GetInst()->Load<CSound>(L"boarSkillActivation", L"sound\\boar\\boar_Skill_Activation.wav");
		m_skillImpactSound = CResourceManager::GetInst()->Load<CSound>(L"boarSkillAttack", L"sound\\boar\\boar_Skill_Attack.wav");
		m_skillHitSound = CResourceManager::GetInst()->Load<CSound>(L"boarSkillHit", L"sound\\boar\\boar_Skill_Hit.wav");
	}
	break;
	case MONSTER_TYPE::MONSTER_CHICKEN:
	{
		m_level = 1;
		m_maxHP = 127.f;
		m_curHP = m_maxHP;
		m_offense = 22.f;
		m_defense = 14.f;
		m_attackSpeed = 0.8f;
		m_moveSpeed = 3.3f;
		m_appear = APPEAR::CHICKEN;

		m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"chickenAttack", L"sound\\chicken\\chickenAttack.wav");
		m_hitSound = CResourceManager::GetInst()->Load<CSound>(L"chickenHit", L"sound\\chicken\\chickenHit.wav");
		m_dieSound = CResourceManager::GetInst()->Load<CSound>(L"chickenDie", L"sound\\chicken\\chickenDie.wav");
		m_wakeUpSound = CResourceManager::GetInst()->Load<CSound>(L"chickenWakeupStart", L"sound\\chicken\\chickenWakeUp_Ing.wav");
	}
	break;
	case MONSTER_TYPE::MONSTER_DOG:
	{
		m_level = 2;
		m_maxHP = 482.f;
		m_curHP = m_maxHP;
		m_offense = 23.f;
		m_defense = 26.f;
		m_attackSpeed = 1.1f;
		m_moveSpeed = 4.f;
		m_appear = APPEAR::WILD_DOG;

		m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"dogAttack", L"sound\\dog\\wildDogAttack.wav");
		m_hitSound = CResourceManager::GetInst()->Load<CSound>(L"dogHit", L"sound\\dog\\wildDogHit.wav");
		m_dieSound = CResourceManager::GetInst()->Load<CSound>(L"dogDie", L"sound\\dog\\wildDogDie.wav");
		m_wakeUpSound = CResourceManager::GetInst()->Load<CSound>(L"dogWakeupStart", L"sound\\dog\\wildDogAttack.wav");
	}
	break;
	case MONSTER_TYPE::MONSTER_WOLF:
	{
		m_level = 3;
		m_maxHP = 638.f;
		m_curHP = m_maxHP;
		m_offense = 44.f;
		m_defense = 28.f;
		m_attackSpeed = 0.9f;
		m_moveSpeed = 4.f;
		m_bSKillExistence = true;
		m_traceDistance = 1200.f;
		m_appear = APPEAR::WOLF;
		
		m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"wolfAttack", L"sound\\wolf\\wolf_attack.wav");
		m_hitSound = CResourceManager::GetInst()->Load<CSound>(L"wolfHit", L"sound\\wolf\\wolfHit.wav");
		m_dieSound = CResourceManager::GetInst()->Load<CSound>(L"wolfDie", L"sound\\wolf\\wolfDie.wav");
		m_skillActiveSound = CResourceManager::GetInst()->Load<CSound>(L"wolfSkillActivation", L"sound\\wolf\\wolf_Skill_Activation_v1.wav");
		m_wakeUpSound = CResourceManager::GetInst()->Load<CSound>(L"wolfWakeupStart", L"sound\\wolf\\wolfWakeUp_Start_v1.wav");
	}
	break;
	case MONSTER_TYPE::END:
		break;
	default:
		break;
	}

	m_monsterType = _type;
}

void CMonsterScript::SetMonsterLv(UINT _Lv)
{
	switch (m_monsterType)
	{
	case MONSTER_TYPE::MONSTER_BAT:
	{
		if (_Lv > 11)
			_Lv = 11;

		_Lv -= m_level;
		m_maxHP += 114.f * _Lv;
		m_curHP = m_maxHP;
		m_offense += 38.f * _Lv;
		m_defense += 4.5f * _Lv;
		m_level += _Lv;
	}
	break;
	case MONSTER_TYPE::MONSTER_BEAR:
	{
		if (_Lv > 24)
			_Lv = 24;

		_Lv -= m_level;
		m_maxHP += 120.f * _Lv;
		m_curHP = m_maxHP;
		m_offense += 35.f * _Lv;
		m_defense += 4.f * _Lv;
		m_moveSpeed += 0.01f * _Lv;
		m_level += _Lv;
		m_specialAttack = m_offense * 2.f;
	}
	break;
	case MONSTER_TYPE::MONSTER_BOAR:
	{
		if (_Lv > 12)
			_Lv = 12;

		_Lv -= m_level;
		m_maxHP += 167.f * _Lv;
		m_curHP = m_maxHP;
		m_offense += 23.f * _Lv;
		m_defense += 6.75f * _Lv;
		m_moveSpeed += 0.03f * _Lv;
		m_level += _Lv;
		m_specialAttack = m_offense * 1.5f;
	}
	break;
	case MONSTER_TYPE::MONSTER_CHICKEN:
	{
		if (_Lv > 7)
			_Lv = 7;

		_Lv -= m_level;
		m_maxHP += 127.f * _Lv;
		m_curHP = m_maxHP;
		m_offense += 25.f * _Lv;
		m_defense += 4.f * _Lv;
		m_moveSpeed += 0.08f * _Lv;
		m_level += _Lv;
	}
	break;
	case MONSTER_TYPE::MONSTER_DOG:
	{
		if (_Lv > 17)
			_Lv = 17;

		_Lv -= m_level;
		m_maxHP += 100.f * _Lv;
		m_curHP = m_maxHP;
		m_offense += 23.f * _Lv;
		m_defense += 4.5f * _Lv;
		m_level += _Lv;
	}
	break;
	case MONSTER_TYPE::MONSTER_WOLF:
	{
		if (_Lv > 20)
			_Lv = 20;

		_Lv -= m_level;
		m_maxHP += 79.f * _Lv;
		m_curHP = m_maxHP;
		m_offense += 22.f * _Lv;
		m_defense += 3.3f * _Lv;
		m_moveSpeed += 0.03f * _Lv;
		m_level += _Lv;
	}
	break;
	case MONSTER_TYPE::END:
		break;
	default:
		break;
	}
}

void CMonsterScript::MonsterRegen()
{
	//해당 몬스터가 플레이어 시야 안에 있는 확인
	// 없을 경우 몬스터 Idle 애니메이션으로 시작,
	// 플레이어 시야 안에 있는 경우 몬스터 재생성 애니메이션 동작

	Object()->SetRenderCheck(true);
	Object()->SetObjOn();

	m_bCharacterDead = false;
	m_bSKillMotion = false;
	m_specialAttack = 0.f;
	m_bDeadAnim = false;
	m_bHpRecovery = false;
	m_curAssaultDistance = 0.f;
	m_AttackCooldownTime = 0.f;
	m_SKillCooldownTime = 0.f;
	m_bAssaultMotion = false;
	m_bReadyMotion = false;
	m_bSKillExistence = false;
	m_bSkillCooldownOn = true;
	m_hpRecoveryCooldownTime = 0.f;
	m_bEndBattleMotion = false;
	m_bRevert = false;
	m_bMonsterMove = true;
	m_bMonsterRenew = true;

	m_curHP = m_maxHP;

	Transform()->SetLocalPos(m_startPos);
	Transform()->SetLocalRot(m_startDir);

	m_monsterState = MONSTER_STATE::MONSTER_IDLE;
	m_trackingUI->SetObjOn();

	if (m_skillEffect_01 != nullptr)
	{
		m_skillEffect_01->SetObjOff();
	}
	if (m_skillEffect_02 != nullptr)
	{
		m_skillEffect_02->SetObjOff();
	}
}

void CMonsterScript::CharacterDead()
{
	// 해당 함수에서 몬스터 매니저에게 자신이 죽었다는 것을 알리기
	CMonsterManager::GetInst()->SetMonsterDead(this);
	SetAnimAction(MONSTER_ANIMCLIP::MONSTER_DEATH, false);

	if(nullptr != m_dieSound && IsPlaySound())
		m_dieSound->Play(1, 0.1f);

	m_monsterState = MONSTER_STATE::MONSTER_DEAD;

	m_attackTarget = nullptr;
	m_trackingUI->SetObjOff();
	m_attackBox->SetObjOff();
	m_bCharacterDead = true;
}

void CMonsterScript::MonsterMove()
{
	// 플레이어 이동 및 그에 따른 회전
	if (nullptr != m_naviMesh)
	{
		// 캐릭터 이동 및 회전
		Vec3 dir = Vec3(0.f, 0.f, 0.f);
		Vec3 pos = Transform()->GetLocalPos();
		Vec3 rot = Transform()->GetLocalRot();
		Vec3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		front.Normalize();
		float distance = 0.f;
		Vec3 waypoint;
		Vec3 pointPos;

		if (nullptr != m_attackTarget)
		{
			pointPos = m_attackTarget->Transform()->GetLocalPos();
		}
		else
		{
			pointPos = m_startPos;
		}

		if (m_bFindPath)
		{
			m_bFindPath = false;
			m_curNode = m_naviMesh->NaviMesh()->GetCurNode(Object());
			m_naviMesh->NaviMesh()->FindPath(m_curNode, Object(), pointPos, m_naviMesh->Transform()->GetWorldMatrix(), m_waypoints);
		}

		if (m_waypoints.size() != 0)
		{
			waypoint = m_waypoints.front();
		}

		Vec3 checkDir = Vec3(0.f, 0.f, 0.f);
		float checkDist = 0.f;

		if (m_bMonsterMove)
		{
			dir.x = waypoint.x - pos.x;
			dir.z = waypoint.z - pos.z;
			distance = dir.Length();

			checkDir.x = pointPos.x - pos.x;
			checkDir.z = pointPos.z - pos.z;
			checkDist = checkDir.Length();
			checkDir.Normalize();
		}

		if (waypoint != pointPos && checkDist < m_traceDistance + 100.f)
		{
			dir = checkDir;
			distance = checkDist;
			waypoint = pointPos;
		}

		if (m_bMonsterRenew)
		{
			m_bRot = true;
		}

		dir.Normalize();

		if (m_bMonsterMove && m_bRot)
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

		if (m_monsterState == MONSTER_STATE::MONSTER_REVERT && Vec3(0.f,0.f,0.f) == dir)
		{
			Transform()->SetLocalPos(m_startPos);
			return;
		}

		Vec3 tempPos = pos;

		pos += dir * m_moveSpeed * 50.f * DELTATIME;

		if (_isnanf(pos.x) || _isnanf(pos.y) || _isnanf(pos.z))
		{
			pos = tempPos;
		}

		m_rotTime += DELTATIME;

		float yAngle = rot.y;

		if (m_rotTime < 0.25f)
			rot.y += m_rotAngle * 4.f * DELTATIME;

		if (_isnanf(rot.y))
			rot.y = yAngle;

		if (m_bMonsterMove && distance < 0.1f)
		{
			m_bMonsterMove = false;
			m_rotAngle = 0.f;
			m_rotTime = 0.f;
			m_bRot = true;

			if (!m_waypoints.empty())
				m_waypoints.pop_front();
		}

		if (!m_bMonsterMove && Vec3::Distance(waypoint, pointPos) > 5.f)
		{
			m_bMonsterMove = true;
			m_bMonsterRenew = true;
		}

		if ((!m_bMonsterMove && Vec3::Distance(waypoint, pointPos) < 5.f) || m_bMonsterRenew)
		{
			m_bFindPath = true;
		}

		if (nullptr != m_curNode)
		{
			m_curNode->SetMove(false);
		}

		bool check = false;
		if (m_naviMesh->NaviMesh()->IsAboveTile(Object(), dir))
		{
			check = true;
			Transform()->SetLocalPos(pos);
			Transform()->SetLocalRot(rot);
		}
	}
}

void CMonsterScript::MonsterItem()
{
	for (int i = 0; i < 5; ++i)
	{
		m_arrBox[i] = ITEM::END;
	}

	for (int i = 0; i < (UINT)ITEM::END; ++i)
	{
		if (ITEM::END != m_arrBox[4])
			break;

		tItem& item = CItemManager::GetInst()->GetItemInfo((ITEM)i);

		for (size_t j = 0; j < item.Appear.size(); ++j)
		{
			if (m_appear != item.Appear[j])
				continue;

			srand((unsigned int)time(NULL));
			int lotto = rand() % 100;
			bool bFull = false;

			if (lotto < 25)
			{
				for (int k = 0; k < 5; ++k)
				{
					if (ITEM::END == m_arrBox[k])
					{
						m_arrBox[k] = (ITEM)i;
						--item.AppearNum;
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	bool empty = false;

	for (int i = 0; i < 5; ++i)
	{
		if (ITEM::END == m_arrBox[i])
		{
			empty = true;
		}
	}

	if (empty)
	{
		for (int i = 0; i < (UINT)ITEM::END; ++i)
		{
			if (ITEM::END != m_arrBox[4])
				break;

			tItem& item = CItemManager::GetInst()->GetItemInfo((ITEM)i);

			for (size_t j = 0; j < item.Appear.size(); ++j)
			{
				if (m_appear != item.Appear[j] || item.AppearNum == 0)
					continue;

				for (int k = 0; k < 5; ++k)
				{
					if (ITEM::END == m_arrBox[k])
					{
						m_arrBox[k] = (ITEM)i;
						--item.AppearNum;
						break;
					}
				}
			}
		}
	}
}

void CMonsterScript::OnColliderEnter(CCollider3D* _other)
{
}

void CMonsterScript::OnCollider(CCollider3D* _other)
{
}

void CMonsterScript::OnColliderExit(CCollider3D* _other)
{
}

void CMonsterScript::Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner)
{
	// 처음 공격 받은 경우 나를 공격한 타겟 넣어주고, IDLE 상태일 때 상대방에 거리에 따라 STAT 넣어줌
	if (MONSTER_STATE::MONSTER_IDLE == m_monsterState)
	{
		// 공격 한 타겟 넣어주기
		m_attackTarget = _owner;

		//타겟 위치와 내 거리 계산을 하여 내 공격 거리에 들어와 있다 == 공격 상태
		//내 거리 밖에 있다. 추격 상태

		//처음 공격 받았을 때 몬스터 타입에 따라 애니메이션 모션 동작해 줘야함
		if(IsPlaySound() && m_wakeUpSound.Get() != nullptr)
			m_wakeUpSound->Play(1, 0.1f);
		// 그냥 바로 TRACE 상태로 변경을 해줄까 ??
		Vec3 targetLenght = Transform()->GetWorldPos_XZ() - m_attackTarget->Transform()->GetWorldPos_XZ();
		if (targetLenght.Length() < m_attackRange)
		{
			m_monsterState = MONSTER_STATE::MONSTER_TRACE;
		}
		else
		{
			m_monsterState = MONSTER_STATE::MONSTER_ATTACK;
		}
	}

	//처음 공격 받는 게 아니면 상대 대미지에 따라 체력 감소
	//HP 다운

	// 맞았는지 확인 및 체력 깎음
	m_bHit = true;

	if (nullptr != m_skillHitSound.Get() && IsPlaySound())
	{
		m_skillHitSound->Play(1, 0.1f);
	}

	float def = m_defense * (1 - m_defReduce);
	float baseDamage = _damage * 100 / (100 + def);
	float extraDamage_percent;
	float finalBaseDamage = (baseDamage * _critical);
	float finalDamage = 0.f;

	extraDamage_percent = 1 + _extraPercent;

	finalDamage = finalBaseDamage * extraDamage_percent + _extraFlat;

	if (finalDamage < 1.f)
	{
		finalDamage = 1.f;
	}

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
	

	m_curHP -= finalDamage;

	CObjectScript::Hit(_type, _damage, _critical, _extraPercent, _extraFlat, _owner);

	if (m_bCharacterDead && _owner->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		((CPlayerScript*)_owner->GetScripts()[0])->AddEXP();
	}
}

void CMonsterScript::Attack()
{
	if (nullptr == m_attackTarget)
		return;

	// 공격 모션 중인지, 공격 쿨이 돌았는지 체크,
	// 공격 쿨이 돌았을 경우 어떤 공격 모션을 사용해야하는지 확인
	// 공격 모션 중일때는 모션의 진행도 확인, 모션이 끝난 경우 대미지 입히고 아닌 경우 넘겨버림
	// 공격 쿨 타임일 때 상대와 거리 비교
	if (m_bAttackMotion)
	{
		// 공격 모션 중일때
		if (m_bAttack && !Animator3D()->IsAnimFinish())
		{
			if (AttackFrameByType())
			{
				if(nullptr != m_attackTarget)
					((CPlayerScript*)m_attackTarget->GetScripts()[0])->Hit(ATTACK_TYPE::BASE, m_offense, 1.f, 0.f, 0.f, Object());

				if(nullptr != m_attackSound.Get() && IsPlaySound())
					m_attackSound->Play(1, 0.1f);

				m_bAttack = false;
			}
		}

		if (!m_bAttack && Animator3D()->IsAnimFinish())
		{
			m_bAttackMotion = false;
			SetAnimAction(MONSTER_ANIMCLIP::MONSTER_WAIT, false);
		}

	}
	else
	{
		Vec3 targetLenght;
		if(nullptr != m_attackTarget)
			 targetLenght = Transform()->GetWorldPos_XZ() - m_attackTarget->Transform()->GetWorldPos_XZ();
		if (targetLenght.Length() > m_attackRange)
		{
			m_monsterState = MONSTER_STATE::MONSTER_TRACE;
			m_bFristAttackMotion = false;
		}
		else
		{
			// 공격 쿨이 돌았을 경우
			if (m_bAttack)
			{
				if (m_bFristAttackMotion)
				{
					// 두번째 공격 모션 실행
					SetAnimAction(MONSTER_ANIMCLIP::MONSTER_ATK02, false);
					m_bFristAttackMotion = false;
				}
				else
				{
					// 첫번째 공격 모션 실행
					SetAnimAction(MONSTER_ANIMCLIP::MONSTER_ATK01, false);
					m_bFristAttackMotion = true;
				}

				m_bAttackMotion = true;
			}
			else
			{
				if (m_attackSpeed < m_AttackCooldownTime)
				{
					m_bAttack = true;
					m_AttackCooldownTime = 0.f;
				}
				else
					m_AttackCooldownTime += DELTATIME;
			}
		}
	}

	Vec3 targetPos = m_attackTarget->Transform()->GetWorldPos_XZ();
	Vec3 pos = Transform()->GetWorldPos_XZ();
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
}

bool CMonsterScript::AttackFrameByType()
{
	switch (m_monsterType)
	{
	case MONSTER_TYPE::MONSTER_BAT:
		if (Animator3D()->GetCurFrameIndex() == 7)
			return true;
		break;
	case MONSTER_TYPE::MONSTER_BEAR:
		if (m_bFristAttackMotion)
		{
			if (Animator3D()->GetCurFrameIndex() == 5)
				return true;
		}
		else
		{
			if (Animator3D()->GetCurFrameIndex() == 10)
				return true;
		}
		break;
	case MONSTER_TYPE::MONSTER_BOAR:
		if (Animator3D()->GetCurFrameIndex() == 5)
			return true;

		break;
	case MONSTER_TYPE::MONSTER_CHICKEN:
		if (m_bFristAttackMotion)
		{
			if (Animator3D()->GetCurFrameIndex() == 7)
				return true;
		}
		else
		{
			if (Animator3D()->GetCurFrameIndex() == 6)
				return true;
		}
		break;
	case MONSTER_TYPE::MONSTER_DOG:
		if (m_bFristAttackMotion)
		{
			if (Animator3D()->GetCurFrameIndex() == 10)
				return true;
		}
		else
		{
			if (Animator3D()->GetCurFrameIndex() == 13)
				return true;
		}
		break;
	case MONSTER_TYPE::MONSTER_WOLF:
		if (m_bFristAttackMotion)
		{
			if (Animator3D()->GetCurFrameIndex() == 11)
				return true;
		}
		else
		{
			if (Animator3D()->GetCurFrameIndex() == 6)
				return true;
		}
		break;
	case MONSTER_TYPE::END:
		break;
	default:
		break;
	}

	return false;
}

void CMonsterScript::Skill()
{
	if (m_bCharacterDead && m_curHP <= 0)
	{
		m_bSKillMotion = false;

		switch (m_monsterType)
		{
		case MONSTER_TYPE::MONSTER_BAT:
			break;
		case MONSTER_TYPE::MONSTER_BEAR:
		{
			m_skillEffect_01->SetObjOff();
		}
			break;
		case MONSTER_TYPE::MONSTER_BOAR:
		{
			m_bReadyMotion = false;
			m_bAssaultMotion = false;
			m_SKillCooldownTime = 0.f;

			m_skillEffect_01->SetObjOff();
			m_skillEffect_02->SetObjOff();
		}
			break;
		case MONSTER_TYPE::MONSTER_CHICKEN:
			break;
		case MONSTER_TYPE::MONSTER_DOG:
			break;
		case MONSTER_TYPE::MONSTER_WOLF:
			break;
		case MONSTER_TYPE::END:
			break;
		default:
			break;
		}

		m_attackTarget = nullptr;
		m_bSkillCooldownOn = false;
		m_bFristAttackMotion = false;
		m_bSKillMotion = false;
		m_monsterState = MONSTER_STATE::MONSTER_DEAD;

		if (m_monsterAnimAction != MONSTER_ANIMCLIP::MONSTER_DEATH)
		{
			SetAnimAction(MONSTER_ANIMCLIP::MONSTER_DEATH, false);
		}
	}

	if (m_bSKillMotion)
	{
		//멧돼지의 경우 Read 애니메이션이 끝나면 돌진 애니메이션으로 변경을 해줘야함
		// 준비 애니메이션 중일때
		if (MONSTER_TYPE::MONSTER_BOAR == m_monsterType)
		{
			if (m_bReadyMotion && !Animator3D()->IsAnimFinish())
			{
				float ratio = (float)Animator3D()->GetCurFrameIndex() / (float)Animator3D()->GetCurAnimMaxFrame();
				m_skillEffect_02->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &ratio);
			}
			else if (m_bReadyMotion && Animator3D()->IsAnimFinish())
			{
				//준비 애니메이션이 끝났을 경우
				m_bReadyMotion = false;
				m_bAssaultMotion = true;

				SetAnimAction(MONSTER_ANIMCLIP::MONSTER_SKILL_ASSAULT, false, 1.f, 2);

				if (nullptr != m_skillImpactSound.Get() && IsPlaySound())
				{
					m_skillImpactSound->Play(1, 0.1f);
				}

				m_skillEffect_01->SetObjOff();
				m_skillEffect_02->SetObjOff();
				// 공격 박스 true로 변경

				m_attackBox->SetOnOff(true);

				float extraSkillDamage = 0.f;
				float skillAmp = 1.f;
				m_attackBox->Collider3D()->SetSphere(false);

				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetHitSound(m_skillHitSound.Get());
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(m_offense);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->Reset();
			}
			else if (m_bAssaultMotion)
			{
				//벽과 충돌했을 시 멈춰야함
				Vec3 pos = Transform()->GetWorldPos();
				pos += (800.f * m_assaultDir * DELTATIME);
				m_curAssaultDistance += 800.f * DELTATIME;
				if (m_curAssaultDistance >= m_assaultDistance || Animator3D()->IsAnimFinish()
					|| !m_naviMesh->NaviMesh()->IsAboveTile(Object(), m_assaultDir) 
					|| ((CHitBoxScript*)m_attackBox->GetScripts()[0])->IsCharacterFirstCol())
				{
					m_bAssaultMotion = false;
					m_bSkillCooldownOn = false;
					m_bFristAttackMotion = false;
					m_SKillCooldownTime = 0.f;
					m_bSKillMotion = false;
					SetAnimAction(MONSTER_ANIMCLIP::MONSTER_WAIT, false);
				}
				else
					Transform()->SetLocalPos(pos);
			}
		}

		if (MONSTER_TYPE::MONSTER_BEAR == m_monsterType && !Animator3D()->IsAnimFinish())
		{
			if (Animator3D()->GetCurFrameIndex() == 2 && !m_skillEffect_01->IsOn())
			{
				m_skillEffect_01->SetObjOn();
			}
			
			if (Animator3D()->GetCurFrameIndex() == 17 && m_skillEffect_01->IsOn())
			{
				m_skillEffect_01->SetObjOff();

				if (nullptr != m_skillImpactSound.Get() && IsPlaySound())
				{
					m_skillImpactSound->Play(1, 0.1f);
				}

				// 여기서 어택박스 생성하기
				m_attackBox->SetOnOff(true);
				float extraSkillDamage = 0.f;
				float skillAmp = 1.f;
				m_attackBox->Collider3D()->SetOffsetPos(Vec3(-385.f, 0.f, -100.f));
				m_attackBox->Collider3D()->SetOffsetScale(Vec3(1.5f, 1.5f, 1.5f));
				m_attackBox->Collider3D()->SetSphere(true);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetHitSound(m_skillHitSound.Get());
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(m_offense);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
			}
		}

		if (Animator3D()->IsAnimFinish())
		{
			m_bSKillMotion = false;
			m_bSkillCooldownOn = false;
			m_bFristAttackMotion = false;
			m_SKillCooldownTime = 0.f;
		}
		//애니메이션 모션이 끝났을 경우 false로 변경
		// 곰의 경우 어택 박스의 콜라이더를 true로 만들어 줘야함
		// 멧돼지의 경우는 차징 준비를 하고 
	}
	else
	{
		// 해당 애니메이션 진행
		switch (m_monsterType)
		{
		case MONSTER_TYPE::MONSTER_BEAR:
		{
			//곰이 공격하는 지점에 attack 박스 만들어준다.
			Vec3 objPos = Object()->Transform()->GetWorldPos_XZ() - m_attackTarget->Transform()->GetWorldPos_XZ();
			objPos.Normalize();

			if (nullptr != m_skillActiveSound.Get() && IsPlaySound())
			{
				m_skillActiveSound->Play(1, 0.1f);
			}

			SetAnimAction(MONSTER_ANIMCLIP::MONSTER_SKILL01, false);
		}
		break;
		case MONSTER_TYPE::MONSTER_BOAR:
		{
			//멧돼지 준비 애니메이션 동작
			m_bReadyMotion = true;
			Vec3 objPos;
			if(nullptr != m_attackTarget)
				objPos = Object()->Transform()->GetWorldPos_XZ() - m_attackTarget->Transform()->GetWorldPos_XZ();
			
			objPos.Normalize();
			m_assaultDir = -objPos;

			m_assaultStratPos = Object()->Transform()->GetWorldPos();
			m_assaultEndPos.XZ() = m_assaultStratPos.XZ() + (m_assaultDir * m_assaultDistance);
			m_assaultEndPos.y = m_assaultStratPos.y;
			SetAnimAction(MONSTER_ANIMCLIP::MONSTER_SKILL_READY, false, 0.8f);
			m_curAssaultDistance = false;

			m_rotAngle = 0.f;
			Vec3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
			front.Normalize();
			Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
			Vec3 crossVec = front.Cross(m_assaultDir);
			float sign = crossVec.Dot(up);

			if (sign > 0.f)
			{
				m_rotAngle += acosf(front.Dot(m_assaultDir)) - XM_PI;
			}
			else if (sign < 0.f)
			{
				m_rotAngle -= acosf(front.Dot(m_assaultDir)) - XM_PI;
			}

			m_rotAngle += Transform()->GetLocalRot().y;

			Transform()->SetLocalRot_Y(m_rotAngle);

			if (nullptr != m_skillActiveSound.Get() && IsPlaySound())
			{
				m_skillActiveSound->Play(1, 0.1f);
			}
			m_skillEffect_01->SetObjOn();
			m_skillEffect_02->SetObjOn();

			float ratio = (float)Animator3D()->GetCurFrameIndex() / (float)Animator3D()->GetCurAnimMaxFrame();
			m_skillEffect_02->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &ratio);
		}
		break;
		case MONSTER_TYPE::MONSTER_WOLF:
		{
			//늑대인 경우 자기 주위에 있는 늑대 모두를 동작 시켜준다.
			SetAnimAction(MONSTER_ANIMCLIP::MONSTER_SKILL01, false);

			if (nullptr != m_skillActiveSound.Get() && IsPlaySound())
			{
				m_skillActiveSound->Play(1, 0.1f);
			}

			CLayer* pLayer = CSceneManager::GetInst()->GetCurScene()->GetLayer(Object()->GetLayerIndex());
			vector<CGameObject*> vecObj = pLayer->GetAllObj();
			for (size_t i = 0; i < vecObj.size(); ++i)
			{
				if (vecObj[i] == Object() || vecObj[i]->GetObjectType() != OBJECT_TYPE::MONSTER || !vecObj[i]->IsOn())
					continue;

				CMonsterScript* pMonsterScript = (CMonsterScript*)vecObj[i]->GetScriptComponent((UINT)SCRIPT_TYPE::MONSTERSCRIPT);
				if (pMonsterScript == nullptr && pMonsterScript->IsCharacterDead() && pMonsterScript->m_attackTarget != nullptr)
					continue;

				Vec3 objPos = Object()->Transform()->GetWorldPos_XZ();
				if (MONSTER_TYPE::MONSTER_WOLF == pMonsterScript->GetMonsterType() && MONSTER_STATE::MONSTER_IDLE == pMonsterScript->GetMonsterState())
				{
					float objDistance = Vec3::Distance(objPos.XZ(), vecObj[i]->Transform()->GetLocalPos().XZ());
					if (objDistance < 800.f)
					{
						pMonsterScript->SetTargetObj(m_attackTarget);
						pMonsterScript->SetMonsterState(MONSTER_STATE::MONSTER_TRACE);
					}
					else
						continue;
					
					// 해당 늑대는 하울링을 안하게 하는 방법 추 후 수정
				}
			}
		}
		break;
		case MONSTER_TYPE::END:
			break;
		default:
			break;
		}

		m_bSKillMotion = true;
	}

}

void CMonsterScript::HpRecovery()
{
	if (m_bHpRecovery)
	{
		m_curHP += m_maxHP / 5;
		if (m_curHP > m_maxHP)
			m_curHP = m_maxHP;

		m_hpRecoveryCooldownTime = 0.f;
		m_bHpRecovery = false;
	}
	else
	{
		if (m_hpRecoveryCooldownTime > 1.f)
		{
			m_bHpRecovery = true;
		}
		else
			m_hpRecoveryCooldownTime += DELTATIME;
	}
}


void CMonsterScript::IdleState()
{
	//자기 기본 애니메이션 동작하게 하면 됨
	SetAnimAction(MONSTER_ANIMCLIP::MONSTER_WAIT, true);

	if (!m_bCharacterDead && nullptr != m_naviMesh && m_naviMesh->NaviMesh()->IsAboveTile(Object(), Vec3(0.f, 0.f, 0.f)))
	{
		m_curNode = m_naviMesh->NaviMesh()->GetCurNode(Object());

		if (nullptr != m_curNode)
		{
			m_curNode->SetMove(false);
		}
	}
}

void CMonsterScript::TraceState()
{
	//A스타 알고리즘 동작
	// 타겟의 방향에 맞게 몬스터가 보는 시선 회전 시켜주기
	float Target = 0.f;
	if (nullptr != m_attackTarget)
		Target = Vec3::Distance(Object()->Transform()->GetWorldPos().XZ(), m_attackTarget->Transform()->GetWorldPos().XZ());
	else
	{
		m_monsterState = MONSTER_STATE::MONSTER_REVERT;
	}

	SetAnimAction(MONSTER_ANIMCLIP::MONSTER_RUN, true);

	MonsterMove();

	float Length = Vec3::Distance(m_startPos.XZ() , Transform()->GetLocalPos().XZ());
	
	if (Target < m_attackRange)
	{
		m_monsterState = MONSTER_STATE::MONSTER_ATTACK;
		m_bFristAttackMotion = false;
	}

	if ((m_traceDistance < Length) || (m_traceDistance < Target))
	{
		m_bHpRecovery = true;
		m_monsterState = MONSTER_STATE::MONSTER_REVERT;
	}
}

void CMonsterScript::AttackState()
{
	if (!m_bSKillMotion && !m_bAttackMotion && 
		nullptr != m_attackTarget && ((CPlayerScript*)m_attackTarget->GetScripts()[0])->IsCharacterDead())
	{
		m_bHpRecovery = true;
		m_monsterState = MONSTER_STATE::MONSTER_REVERT;
	}

	//스킬 존재 여부에 따라 공격 방법 정하기
	if (m_bSKillExistence)
	{
		if (m_bSkillCooldownOn || m_bSKillMotion)
		{
			Skill();
		}
		else
			Attack();
	}
	else
	{
		Attack();
	}
}

void CMonsterScript::RevertState()
{
	//a스타
	// 자신이 원래 있었던 위치로 되돌아가게 하기

	if (!m_bEndBattleMotion && !m_bRevert)
	{
		SetAnimAction(MONSTER_ANIMCLIP::MONSTER_ENDBATTLE, false);
		m_bEndBattleMotion = true;
		m_attackTarget = nullptr;
	}
	else
	{
		if (m_bEndBattleMotion && !m_bRevert && Animator3D()->IsAnimFinish())
		{
			m_bRevert = true;
			SetAnimAction(MONSTER_ANIMCLIP::MONSTER_RUN, true);
		}
			
		else
		{
			MonsterMove();

			Vec3 pos = Object()->Transform()->GetWorldPos_XZ();
			if (Object()->Transform()->GetWorldPos_XZ() == m_startPos.XZ())	// 몬스터 바라보는 방향도 돌려야 함 (확인 필요)
			{
				m_monsterState = MONSTER_STATE::MONSTER_IDLE;
			}

			if (Vec3::Distance(m_startPos.XZ(), Transform()->GetWorldPos_XZ()) < 10.f)
			{
				Object()->Transform()->SetLocalPos(m_startPos);
			}
		}
			
	}

	HpRecovery();
}

void CMonsterScript::DeadState()
{
	// 죽는 애니메이션 동작
	// 한번만 동작하게 하기
	if (!m_bDeadAnim)
	{
		m_bDeadAnim = true;
		SetAnimAction(MONSTER_ANIMCLIP::MONSTER_DEATH, false);
	}
	if (nullptr != m_skillEffect_01 && m_skillEffect_01->IsOn())
		m_skillEffect_01->SetObjOff();

	if (nullptr != m_skillEffect_02 && m_skillEffect_02->IsOn())
		m_skillEffect_02->SetObjOff();

	if (!Animator3D()->IsAnimFinish() && MONSTER_TYPE::MONSTER_BAT == m_monsterType)
	{
		if (21 > Animator3D()->GetCurFrameIndex())
		{
			float fPosY = Transform()->GetLocalPos().y;
			fPosY -= DELTATIME * 500.f;

			if (fPosY < 70.f)
				fPosY = 70.f;
			Transform()->SetLocalPos_Y(fPosY);
		}
		else if (21 == Animator3D()->GetCurFrameIndex())
			Transform()->SetLocalPos_Y(70.f);
	}

	m_curHP = -1.f;
}

void CMonsterScript::SetAnimAction(MONSTER_ANIMCLIP _action, bool _bLoop ,float _animSpeed, int _count)
{
	if (m_monsterAnimAction == _action)
	{
		if (_bLoop != Animator3D()->GetRepeat())
			Animator3D()->SetRepeat(_bLoop, _count);
		return;
	}

	wstring animName;
	switch (_action)
	{
	case MONSTER_ANIMCLIP::MONSTER_ATK01:
		animName = L"atk01";
		break;
	case MONSTER_ANIMCLIP::MONSTER_DEATH:
		animName = L"death";
		break;
	case MONSTER_ANIMCLIP::MONSTER_RUN:
		animName = L"run";
		break;
	case MONSTER_ANIMCLIP::MONSTER_WAIT:
		animName = L"wait";
		break;
	case MONSTER_ANIMCLIP::MONSTER_ATK02:
		animName = L"atk02";
		break;
	case MONSTER_ANIMCLIP::MONSTER_APPEAR:
		animName = L"appear";
		break;
	case MONSTER_ANIMCLIP::MONSTER_ENDBATTLE:
		animName = L"endbattle";
		break;
	case MONSTER_ANIMCLIP::MONSTER_BEWARE_START:
		animName = L"beware_start";
		break;
	case MONSTER_ANIMCLIP::MONSTER_BEWARE_END:
		animName = L"beware_end";
		break;
	case MONSTER_ANIMCLIP::MONSTER_BEWARE_LOOP:
		animName = L"beware_loop";
		break;
	case MONSTER_ANIMCLIP::MONSTER_DANCE:
		animName = L"dance";
		break;
	case MONSTER_ANIMCLIP::MONSTER_WAKE:
		animName = L"wake";
		break;
	case MONSTER_ANIMCLIP::MONSTER_SLEEP_START:
		animName = L"sleep_start";
		break;
	case MONSTER_ANIMCLIP::MONSTER_SLEEP:
		animName = L"sleep";
		break;
	case MONSTER_ANIMCLIP::MONSTER_SKILL01:
		animName = L"skill01";
		break;
	case MONSTER_ANIMCLIP::MONSTER_APPEARWAIT:
		animName = L"appearwait";
		break;
	case MONSTER_ANIMCLIP::MONSTER_SKILL_READY:
		animName = L"skill01_ready";
		break;
	case MONSTER_ANIMCLIP::MONSTER_SKILL_ASSAULT:
		animName = L"skill01_assault";
		break;
	case MONSTER_ANIMCLIP::END:
		return;
	default:
		return;
	}
	Animator3D()->Play(animName, _bLoop, _animSpeed, _count);
	m_monsterAnimAction = _action;
}

ITEM& CMonsterScript::GetBoxItem(int _index)
{
	if (_index > 4 || _index < 0)
	{
		ITEM item = ITEM::END;
		return item;
	}

	return m_arrBox[_index];
}

void CMonsterScript::LootEnd(int _index)
{
	if (_index > 4 || _index < 0)
	{
		return;
	}

	m_arrBox[_index] = ITEM::END;
}

void CMonsterScript::SaveToScene(FILE* _file)
{
	CObjectScript::SaveToScene(_file);

	SaveToFile(&m_startPos, _file);
	SaveToFile(&m_startDir, _file);
	SaveToFile(&m_specialAttack, _file);
	SaveToFile(&m_monsterType, _file);

}

void CMonsterScript::LoadFromScene(FILE* _file)
{
	CObjectScript::LoadFromScene(_file);

	LoadFromFile(&m_startPos, _file);
	LoadFromFile(&m_startDir, _file);
	LoadFromFile(&m_specialAttack, _file);
	LoadFromFile(&m_monsterType, _file);

	m_CurPos = m_startPos;
	m_startPosDir = m_startDir;
}
