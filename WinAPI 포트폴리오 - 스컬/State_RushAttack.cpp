#include "stdafx.h"
#include "State_RushAttack.h"
#include "SceneManager.h"
#include "HitBox.h"

State_RushAttack::State_RushAttack() :
	m_target(nullptr),
	m_bDash(false)
{
}

State_RushAttack::~State_RushAttack()
{
}

void State_RushAttack::update()
{
	float dir = GetMonster()->GetDir();
	GetMonster()->SetAnimDir(dir, L"Dash", true);

	if (!m_bDash)
	{
		GetMonster()->AddVelocity(Vector2(dir * 1700.f, 0.f));
		GetMonster()->SetSubSpeed(120.f);
		m_bDash = true;
	}

	if (GetMonster()->GetVelocity() == Vector2(0.f, 0.f))
	{
		ChangeState(L"Idle");
		m_bDash = false;
	}
}

void State_RushAttack::lateUpdate()
{
}

void State_RushAttack::Enter()
{
	float dir = GetMonster()->GetDir();

	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}

	GetMonster()->CreateHitBox(Vector2(dir * 60.f, -80.f), Vector2(70.f, 150.f));
	PlaySoundEffect(L"TackleReady", L"Sound\\MAA_Tackle_Ready.wav", false, 0.f, 40.f);
}

void State_RushAttack::Exit()
{
	HitBox* hitBox = GetMonster()->GetHitBox();

	if (nullptr != hitBox)
	{
		DeleteObject(hitBox);
		GetMonster()->InitHitBox();
	}

	m_bDash = false;
}
