#include "pch.h"
#include "CObjectScript.h"
#include "CHitBoxScript.h"
#include "CUIScript.h"
#include "CMouseRay.h"

#include <Engine\CRenderManager.h>
#include <Engine\CCollider3D.h>
#include <Engine\CNaviMesh.h>

CGameObject* CObjectScript::g_itemBox = nullptr;

CObjectScript::CObjectScript(UINT _type)
	: CScript(_type),
	m_curNode(nullptr),
	m_trackingUI(nullptr),
	m_level(7),
	m_maxLevel(20),
	m_maxHP(100.f),
	m_curHP(m_maxHP),
	m_maxStamina(100.f),
	m_curStamina(m_maxStamina),
	m_offense(10.f),
	m_defense(10.f),
	m_moveSpeed(100.f),
	m_healthRegen(1.f),
	m_staminaRegen(1.f),
	m_attackSpeed(1.f),
	m_criticalChance(0.f),
	m_criticalDamage(1.65f),
	m_FOV(8.f),
	m_rotAngle(0.f),
	m_rotTime(0.f),
	m_stunTime(0.f),
	m_slowTime(0.f),
	m_silenceTime(0.f),
	m_nonMoveTime(0.f),
	m_defReduceTime(0.f),
	m_defReduce(0.f),
	m_slowRate(0.f),
	m_defensePenetration(0.f),
	m_attackRange(200.f),
	m_visionRange(2000.f),
	m_defense_Buff(0.f),
	m_bAttack(false),
	m_bRot(true),
	m_bCharacterDead(false),
	m_bHit(false),
	m_bStun(false),
	m_bSlow(false),
	m_bSilence(false),
	m_bNonStop(false),
	m_bDefReduce(false),
	m_bNonMove(false),
	m_bFindPath(true),
	m_bBuff(false),
	m_bAICheck(true),
	m_attackTarget(nullptr),
	m_bFristAttackMotion(false),
	m_bAttackMotion(false),
	m_bMotion(false),
	m_attackBox(nullptr),
	m_naviMesh(nullptr),
	m_hitEffect(nullptr),
	m_mouse(nullptr)
{
	
}

CObjectScript::CObjectScript(const CObjectScript& _origin)
	: CScript(_origin),
	m_curNode(nullptr),
	m_trackingUI(nullptr),
	m_level(_origin.m_level),
	m_maxLevel(20),
	m_maxHP(_origin.m_maxHP),
	m_curHP(m_maxHP),
	m_maxStamina(_origin.m_maxStamina),
	m_curStamina(m_maxStamina),
	m_offense(_origin.m_offense),
	m_defense(_origin.m_defense),
	m_moveSpeed(_origin.m_moveSpeed),
	m_healthRegen(_origin.m_healthRegen),
	m_staminaRegen(_origin.m_staminaRegen),
	m_attackSpeed(_origin.m_attackSpeed),
	m_criticalChance(_origin.m_criticalChance),
	m_criticalDamage(_origin.m_criticalDamage),
	m_FOV(8.f),
	m_rotAngle(0.f),
	m_rotTime(0.f),
	m_stunTime(0.f),
	m_slowTime(0.f),
	m_silenceTime(0.f),
	m_nonMoveTime(0.f),
	m_defReduceTime(0.f),
	m_defReduce(0.f),
	m_slowRate(0.f),
	m_defensePenetration(0.f),
	m_attackRange(_origin.m_attackRange),
	m_visionRange(_origin.m_visionRange),
	m_defense_Buff(0.f),
	m_bAttack(false),
	m_bRot(true),
	m_bCharacterDead(false),
	m_bHit(false),
	m_bStun(false),
	m_bSlow(false),
	m_bSilence(false),
	m_bNonStop(false),
	m_bDefReduce(false),
	m_bNonMove(false),
	m_bFindPath(true),
	m_bBuff(false),
	m_bAICheck(true),
	m_attackTarget(nullptr),
	m_bFristAttackMotion(false),
	m_bAttackMotion(false),
	m_bMotion(false),
	m_attackBox(nullptr),
	m_hitEffect(nullptr),
	m_naviMesh(_origin.m_naviMesh),
	m_mouse(nullptr)
{
}

CObjectScript::~CObjectScript()
{
}

void CObjectScript::start()
{
	Object()->SetDynamic(true);

	StatsReSet();

	m_naviMesh = CSceneManager::GetInst()->FindObjByName(L"NaviMesh");
	m_mouse = CSceneManager::GetInst()->FindObjByName(L"Mouse");

	vector<CGameObject*> childObj = Object()->GetChilds();

	if (nullptr == m_hitEffect)
	{
		m_hitEffect = CResourceManager::GetInst()->Load<CPrefab>(L"HitEffect", L"prefab\\HitEffect.prefab").Get();
	}

	if (nullptr == m_attackBox)
	{
		if (!childObj.empty())
		{
			for (int i = 0; i < childObj.size(); ++i)
			{
				if (L"AttackBox" == childObj[i]->GetName())
				{
					m_attackBox = childObj[i];
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetOwner(Object());

					break;
				}
			}
		}

		if (nullptr == m_attackBox)
		{
			m_attackBox = new CGameObject;
			m_attackBox->SetName(L"AttackBox");
			m_attackBox->AddComponent(new CTransform);
			m_attackBox->AddComponent(new CCollider3D);
			m_attackBox->AddComponent(new CMeshRender);
			m_attackBox->AddComponent(new CHitBoxScript);
			m_attackBox->SetOnOff(true);
			m_attackBox->Collider3D()->SetSphere(true);

			((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetOwner(Object());
			CScript::AddChild(Object(), m_attackBox);
		}
	}

	m_naviMesh = CSceneManager::GetInst()->FindObjByName(L"NaviMesh");

	if (nullptr != m_attackBox)
		m_attackBox->SetObjOff();

	if (nullptr == g_itemBox)
	{
		Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"ItemBoxUI", L"prefab\\RootItemUI.prefab");
		g_itemBox = prefab->Instantiate();
		g_itemBox->SetOnOff(false);
		for (size_t i = 0; i < g_itemBox->GetChilds().size(); ++i)
		{
			((CUIScript*)g_itemBox->GetChilds()[i]->GetScripts()[0])->SetTarget(Object());
		}
		CScript::CreateObject(g_itemBox, 30);
	}

	m_bAICheck = true;
}

void CObjectScript::update()
{
	if ((nullptr != m_attackTarget && ((CObjectScript*)m_attackTarget->GetScripts()[0])->m_bCharacterDead) || m_bCharacterDead)
	{
		m_attackTarget = nullptr;
	}

	CGameObject* target = nullptr;
	CGameObject* player = nullptr;
	float dist = 10000.f;

	if (IsMouseClicked(MOUSE_BUTTON::RIGHT) && nullptr != m_mouse)
	{
		target = ((CMouseRay*)m_mouse->GetScripts()[0])->MousePicking();

		if (nullptr != target && (OBJECT_TYPE::PLAYER == target->GetObjectType() || OBJECT_TYPE::MONSTER == target->GetObjectType()))
		{
			for (size_t i = 0; i < g_itemBox->GetChilds().size(); ++i)
			{
				((CUIScript*)g_itemBox->GetChilds()[i]->GetScripts()[0])->SetTarget(target);
			}
		}
		player = CSceneManager::GetInst()->FindObjByName(L"Player");
	}

	if (Object() != player && nullptr != player && nullptr != target && m_bCharacterDead)
	{
		Vec3 targetPos = target->Transform()->GetWorldPos_XZ();
		Vec3 playerPos = player->Transform()->GetWorldPos_XZ();

		dist = Vec3::Distance(targetPos, playerPos);

		if (Object() == target && dist < 400.f)
		{
			// 아이템 UI 오픈
			g_itemBox->SetOnOff(true);
		}
	}

	if (nullptr != g_itemBox && g_itemBox->IsOn() && IsMouseClicked() && nullptr == CUIManager::GetInst()->GetUI() && dist > 100.f)
	{
		// 아이템 UI 닫기
		g_itemBox->SetOnOff(false);
	}

	if (m_bCharacterDead)
	{
		m_attackBox->SetOnOff(false);
		
		if(nullptr != m_curNode)
			m_curNode->SetMove(true);
	}

	if (m_bNonStop)
	{
		m_bStun = false;
		m_bSlow = false;
		m_bSilence = false;
	}

	if (m_bStun)
	{
		m_stunTime -= DELTATIME;

		if (m_stunTime < 0.f)
		{
			m_stunTime = 0.f;
			m_bStun = false;
		}
	}

	if (m_bSlow)
	{
		m_slowTime -= DELTATIME;

		if (m_slowTime < 0.f)
		{
			m_slowTime = 0.f;
			m_slowRate = 0.f;
			m_bSlow = false;
		}
	}

	if (m_bSilence)
	{
		m_silenceTime -= DELTATIME;

		if (m_silenceTime < 0.f)
		{
			m_silenceTime = 0.f;
			m_bSilence = false;
		}
	}

	if (m_bDefReduce)
	{
		m_defReduce -= DELTATIME;

		if (m_defReduce < 0.f)
		{
			m_defReduce = 0.f;
			m_bDefReduce = false;
		}
	}

	if (m_bNonMove)
	{
		m_nonMoveTime -= DELTATIME;

		if (m_nonMoveTime < 0.f)
		{
			m_nonMoveTime = 0.f;
			m_bNonMove = false;
		}
	}

	if (m_bHit)
	{
		m_bHit = false;
	}
}

void CObjectScript::Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner)
{
	// 체력이 0 이하면 죽음
	if (m_curHP <= 0.f)
	{
		m_curHP = 0.f;
		m_bCharacterDead = true;
		CharacterDead();
	}
	else
	{
		if(Object()->GetObjectType() == OBJECT_TYPE::MONSTER && nullptr != m_hitSound.Get() && IsPlaySound())
			m_hitSound->Play(1, 0.1f);
	}
		
}

bool CObjectScript::IsPlaySound()
{
	// 실제 start 되었을 때 사용
	CGameObject* mainCam = CSceneManager::GetInst()->FindObjByName(L"MainCamera");

	if (nullptr != mainCam)
	{
		Vec3 pos = Object()->Transform()->GetLocalPos().XZ() - mainCam->Transform()->GetLocalPos().XZ();
		if (pos.Length() < 1000.f)
			return true;
		else
			return false;
	}

	return false;
}

void CObjectScript::SaveToScene(FILE* _file)
{
}

void CObjectScript::LoadFromScene(FILE* _file)
{
}
