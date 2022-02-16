#include "stdafx.h"
#include "State_Attack.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Monster.h"
#include "HitBox.h"
#include "Animator.h"
#include "Animation.h"

State_Attack::State_Attack() :
	m_target(nullptr)
{
}

State_Attack::~State_Attack()
{
}

void State_Attack::update()
{
	assert(m_target);

	Vector2 targetPos = m_target->GetPos();
	Vector2 pos = GetMonster()->GetPos();

	Vector2 diff = targetPos - pos;

	if (0 > diff.x)
		GetMonster()->SetDir(-1.f);
	else
		GetMonster()->SetDir(1.f);


	if (50.f < abs(diff.x) || 30.f < abs(diff.y))
	{
		
		m_term -= DELTATIME;

		if (m_term < 0.f)
		{
			ChangeState(L"Trace");
		}
	}

	Animation* anim = GetMonster()->GetAnimator()->GetCurAnim();

	if (anim->GetCurFrameIdx() == 0)
	{
		HitBox* hitBox = GetMonster()->GetHitBox();

		if (nullptr == hitBox)
		{
			float dir = GetMonster()->GetDir();

			if (GetMonster()->GetMonsterType() == MONSTER_GROUP::FOOTMAN)
			{
				GetMonster()->CreateHitBox(Vector2(dir * 30.f, -70.f), Vector2(70.f, 70.f));
			}
			else if (GetMonster()->GetMonsterType() == MONSTER_GROUP::HEAVYINFANTRY)
			{
				GetMonster()->CreateHitBox(Vector2(dir * 40.f, -90.f), Vector2(150.f, 150.f));
			}
		}
	}
}

void State_Attack::lateUpdate()
{
	Vector2 targetPos = m_target->GetPos();
	Vector2 pos = GetMonster()->GetPos();

	Vector2 dir = targetPos - pos;

	if (0 > dir.x)
		GetMonster()->SetDir(-1.f);
	else
		GetMonster()->SetDir(1.f);

	if (GetMonster()->GetMonsterType() == MONSTER_GROUP::FOOTMAN)
	{
		if (50.f > abs(dir.x) && 30.f > abs(dir.y))
		{
			GetMonster()->SetAnimDir(GetMonster()->GetDir(), L"Attack", true);
		}
	}
	else if (GetMonster()->GetMonsterType() == MONSTER_GROUP::HEAVYINFANTRY)
	{
		if (70.f > abs(dir.x) && 50.f > abs(dir.y))
		{
			GetMonster()->SetAnimDir(GetMonster()->GetDir(), L"Attack", true);
		}
	}

	Animation* anim = GetMonster()->GetAnimator()->GetCurAnim();

	if (anim->IsFinish())
	{
		HitBox* hitBox = GetMonster()->GetHitBox();

		if (nullptr != hitBox)
		{
			DeleteObject(hitBox);
			GetMonster()->InitHitBox();
		}
	}
}

void State_Attack::Enter()
{
	float dir = GetMonster()->GetDir();

	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}

	if (GetMonster()->GetMonsterType() == MONSTER_GROUP::FOOTMAN)
	{
		GetMonster()->CreateHitBox(Vector2(dir * 30.f, -70.f), Vector2(70.f, 70.f));
		PlaySoundEffect(L"AttackReady", L"Sound\\Atk_Ready.wav", false, 0.f, 40.f);
	}
	else if (GetMonster()->GetMonsterType() == MONSTER_GROUP::HEAVYINFANTRY)
	{
		GetMonster()->CreateHitBox(Vector2(dir * 40.f, -90.f), Vector2(150.f, 150.f));
		PlaySoundEffect(L"MAAAttackReady", L"Sound\\MAA_Atk_Ready.wav", false, 0.f, 40.f);
	}
}

void State_Attack::Exit()
{
	HitBox* hitBox = GetMonster()->GetHitBox();

	if (nullptr != hitBox)
	{
		DeleteObject(hitBox);
		GetMonster()->InitHitBox();
	}
}
