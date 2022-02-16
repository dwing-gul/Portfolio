#include "stdafx.h"
#include "State_Idle.h"
#include "Boss.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "State_Trace.h"
#include "Animator.h"
#include "AI.h"
#include "State.h"

State_Idle::State_Idle() :
	m_target(nullptr)
{
}

State_Idle::~State_Idle()
{
}

void State_Idle::update()
{
	assert(m_target);

	if (MONSTER_GROUP::BOSS == GetMonster()->GetMonsterType())
	{
		GetMonster()->GetAnimator()->PlayAnim(L"Idle", true);

		m_term -= DELTATIME;

		if (m_term < 0.f)
		{
			m_term = 2.f;
			srand((unsigned int)time(0));

			float temp = (float)rand();
			float percent = rand() % 10000 / 100.f;
			wstring state = L"";

			if (GetMonster()->GetName() == L"BossBody")
			{
				if (percent <= 33.3f)
				{
					state = L"ShotMissile";
				}
				else if (percent > 33.3f && percent <= 66.6f)
				{
					state = L"Sweep";
				}
				else
				{
					state = L"Stamp";
				}

				vector<Boss*> childParts = ((Boss*)GetMonster())->GetChildParts();

				for (int i = 0; i < childParts.size(); ++i)
				{
					childParts[i]->GetAI()->ChangeState(state);
				}

				((Boss*)GetMonster())->GetLeftHand()->GetAI()->ChangeState(state);
				((Boss*)GetMonster())->GetRightHand()->GetAI()->ChangeState(state);

				ChangeState(state);
			}
		}
	}
	else
	{
		Vector2 diff = m_target->GetPos() - GetMonster()->GetPos();

		if (0 > diff.x)
			GetMonster()->SetDir(-1.f);
		else
			GetMonster()->SetDir(1.f);

		GetMonster()->SetAnimDir(GetMonster()->GetDir(), L"Idle", true);

		if (MONSTER_GROUP::FOOTMAN == GetMonster()->GetMonsterType())
		{
			if (200.f > abs(diff.x) && 30.f > abs(diff.y))
			{

				m_term -= DELTATIME;

				if (m_term < 0.f)
				{
					m_term = 0.7f;
					ChangeState(L"Trace");
				}
			}
		}
		else if (MONSTER_GROUP::HEAVYINFANTRY == GetMonster()->GetMonsterType())
		{
			if (800.f > abs(diff.x) && 100.f > abs(diff.y))
			{
				srand((unsigned int)time(NULL));

				int temp = rand();
				float percent = rand() % 10000 / 100.f;

				m_term -= DELTATIME;

				if (m_term < 0.f)
				{
					m_term = 0.7f;
					if (percent < 50.f)
						ChangeState(L"Trace");
					else
						ChangeState(L"RushAttack");
				}
			}
		}
	}
}

void State_Idle::lateUpdate()
{
}

void State_Idle::Enter()
{
	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}
}

void State_Idle::Exit()
{
}
