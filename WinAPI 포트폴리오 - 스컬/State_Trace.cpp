#include "stdafx.h"
#include "State_Trace.h"
#include "SceneManager.h"
#include "Monster.h"
#include "TimeManager.h"

State_Trace::State_Trace() :
	m_target(nullptr)
{
}

State_Trace::~State_Trace()
{
}

void State_Trace::update()
{
	assert(m_target);

	Vector2 targetPos = m_target->GetPos();
	Vector2 pos = GetMonster()->GetPos();

	Vector2 dir = targetPos - pos;
	Vector2 diff = dir;
	dir.normalize();


	if (150.f < abs(diff.x) || 30.f < abs(diff.y))
	{
		
		m_term -= DELTATIME;

		if (m_term < 0.f)
		{
			ChangeState(L"Idle");
		}
	}
	else 
	{
		if (50.f > abs(diff.x))
		{
			
			m_term -= DELTATIME;

			if (m_term < 0.f)
			{
				ChangeState(L"Attack");
			}
		}
		else
		{
			pos.x += dir.x * 100.f * DELTATIME;
			GetMonster()->SetPos(pos);
		}
	}
}

void State_Trace::lateUpdate()
{
	Vector2 targetPos = m_target->GetPos();
	Vector2 pos = GetMonster()->GetPos();

	Vector2 dir = targetPos - pos;

	if (0 > dir.x)
		GetMonster()->SetDir(-1.f);
	else
		GetMonster()->SetDir(1.f);

	if (150.f > abs(dir.x) && 30.f > abs(dir.y))
	{
		GetMonster()->SetAnimDir(GetMonster()->GetDir(), L"Walk", true);
	}
}

void State_Trace::Enter()
{
	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}
}

void State_Trace::Exit()
{
}
