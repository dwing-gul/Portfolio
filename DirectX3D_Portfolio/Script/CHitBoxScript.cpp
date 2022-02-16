#include "pch.h"
#include "CHitBoxScript.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"

#include <Engine/CCollider3D.h>
#include <Engine/CRenderManager.h>

CHitBoxScript::CHitBoxScript() :
	CScript((UINT)SCRIPT_TYPE::HITBOXSCRIPT),
	m_owner(nullptr),
	m_damage(0.f),
	m_critical(0.f),
	m_extraPercent(0.f),
	m_extraFlat(0.f),
	m_criticalChance(0.f),
	m_debuff(0),
	m_debuffTime(0.f),
	m_debuffRate(0.f),
	m_bRate(false),
	m_bCharacterFirstCol(false),
	m_fristTarget(nullptr)
{
}

CHitBoxScript::~CHitBoxScript()
{
}

void CHitBoxScript::start()
{
}

void CHitBoxScript::update()
{
}

void CHitBoxScript::lateUpdate()
{
}

void CHitBoxScript::OnColliderEnter(CCollider3D* _other)
{
	CGameObject* target = _other->Object();
	if (nullptr == m_owner)
		return;

	if (target == m_owner && (target->GetObjectType() != OBJECT_TYPE::PLAYER &&
		target->GetObjectType() != OBJECT_TYPE::MONSTER))
		return;

	if (((CObjectScript*)target->GetScripts()[0])->IsCharacterDead())
		return;

	if (m_bRate)
	{
		m_damage += ((CObjectScript*)target->GetScripts()[0])->GetCurHP() * 0.1f;
	}

	if (nullptr != m_skillHitSound.Get() && IsPlaySound())
	{
		m_skillHitSound->Play(1, 0.1f);
	}

	m_bCharacterFirstCol = true;
	m_fristTarget = target;

	srand((unsigned int)time(NULL));
	int criticalPercent = rand() % 100;
	bool bCritical = false;

	if (criticalPercent < int(m_criticalChance * 100.f))
		bCritical = true;

	if (target->GetObjectType() == OBJECT_TYPE::MONSTER)
	{
		if (!((CMonsterScript*)target->GetScripts()[0])->IsHit())
		{
			if (bCritical)
			{
				((CMonsterScript*)target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_damage, m_critical, m_extraPercent, m_extraFlat, m_owner);
			}
			else
			{
				((CMonsterScript*)target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_damage, 1.f, m_extraPercent, m_extraFlat, m_owner);
			}

			CheckDebuff(((CMonsterScript*)target->GetScripts()[0]));
		}
	}
	else if (m_owner != target && target->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		if (!((CPlayerScript*)target->GetScripts()[0])->IsHit())
		{
			if (bCritical)
			{
				((CPlayerScript*)target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_damage, m_critical, m_extraPercent, m_extraFlat, m_owner);
			}
			else
			{
				((CPlayerScript*)target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_damage, 1.f, m_extraPercent, m_extraFlat, m_owner);
			}

			CheckDebuff(((CPlayerScript*)target->GetScripts()[0]));
		}
	}
}

void CHitBoxScript::OnCollider(CCollider3D* _other)
{
}

void CHitBoxScript::OnColliderExit(CCollider3D* _other)
{
	m_bCharacterFirstCol = false;
	m_fristTarget = nullptr;
}

void CHitBoxScript::SetDebuff(DEBUFF_TYPE _debuff, bool _bAdapt)
{
	if(_bAdapt)
		m_debuff |= (UINT)_debuff;
	else
		m_debuff &= ~((UINT)_debuff);
}

void CHitBoxScript::Reset()
{
	m_bCharacterFirstCol = false;
}

bool CHitBoxScript::IsPlaySound()
{
	//CGameObject* mainCam = CRenderManager::GetInst()->GetMainCam()->Object();

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

void CHitBoxScript::CheckDebuff(CObjectScript* _target)
{
	if (m_debuff & (UINT)DEBUFF_TYPE::SLOW)
	{
		_target->SetSlow(true, m_debuffTime, m_debuffRate);
	}

	if (m_debuff & (UINT)DEBUFF_TYPE::STUN)
	{
		_target->SetStun(true, m_debuffTime);
	}

	if (m_debuff & (UINT)DEBUFF_TYPE::SILENCE)
	{
		_target->SetSilence(true, m_debuffTime);
	}

	if (m_debuff & (UINT)DEBUFF_TYPE::NONMOVE)
	{
		_target->SetNonMove(true, m_debuffTime);
	}

	if (m_debuff & (UINT)DEBUFF_TYPE::DEF_REDUCE)
	{
		_target->SetDefReduce(true, m_debuffTime, m_debuffRate);
	}
}
