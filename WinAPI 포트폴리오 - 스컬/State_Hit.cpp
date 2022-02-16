#include "stdafx.h"
#include "State_Hit.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Animation.h"
#include "Animator.h"

State_Hit::State_Hit() :
	m_target(nullptr),
	m_hitCount(0)
{
}

State_Hit::~State_Hit()
{
}

void State_Hit::update()
{
	assert(m_target);

	float dir = GetMonster()->GetDir();

	
	GetMonster()->SetAnimDir(dir, L"Hit", false);
}

void State_Hit::lateUpdate()
{
	Animation* curAnim = GetMonster()->GetAnimator()->GetCurAnim();

	if (curAnim->IsFinish() && !GetMonster()->IsHit())
	{
		curAnim->SetFrameIdx(0);

		m_term -= DELTATIME;

		if (m_term < 0.f)
		{
			ChangeState(L"Idle");
		}
	}
}

void State_Hit::Enter()
{
	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}
}

void State_Hit::Exit()
{

}
