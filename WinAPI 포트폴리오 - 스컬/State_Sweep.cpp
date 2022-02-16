#include "stdafx.h"
#include "State_Sweep.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Boss.h"
#include "HitBox.h"
#include "Animator.h"
#include "Animation.h"
#include "Camera.h"

int State_Sweep::m_count = 2;
bool State_Sweep::g_bLeftHandFinish = false;
bool State_Sweep::g_bRightHandFinish = false;

State_Sweep::State_Sweep() :
	m_target(nullptr),
	m_bReady(false),
	m_bInit(true),
	m_bSoundPlay(false),
	m_dir(1),
	m_distance(500.f),
	m_calDistance(0.f),
	m_speed(200.f),
	m_bLeft(false),
	m_bRight(false)
{
}

State_Sweep::~State_Sweep()
{
}

void State_Sweep::update()
{
	if (m_bInit)
	{
		m_prevPos = GetMonster()->GetPos();
		((Boss*)GetMonster())->SetPrevPos(m_prevPos);
		m_count = 2;
	}

	if (!m_bReady)
	{
		m_bInit = false;

		if (GetMonster()->GetName() == L"LeftHand")
		{
			m_dir = 1;
		}
		else if (GetMonster()->GetName() == L"RightHand")
		{
			m_dir = -1;
		}

		if (GetMonster()->GetName() == L"LeftHand" || GetMonster()->GetName() == L"RightHand")
		{
			Vector2 pos = GetMonster()->GetPos();

			pos.x += m_dir * m_speed * DELTATIME;
			m_calDistance += m_speed * DELTATIME;
			GetMonster()->SetPos(pos);

			GetMonster()->GetAnimator()->PlayAnim(L"Sweep", false);
			if (m_calDistance >= m_distance)
			{
				m_bReady = true;
				m_calDistance = 0.f;
				m_startPos = pos;
			}
		}
		else
		{
			m_bReady = true;
		}

		((Boss*)GetMonster())->SweepHandSelect();

		if (((Boss*)GetMonster())->IsLeft())
		{
			m_bLeft = true;
		}
		else
		{
			m_bRight = true;
		}
	}
	else
	{
		if (0 < m_count)
		{
			if (GetMonster()->GetName() == L"LeftHand" && ((Boss*)GetMonster())->IsLeft() && m_bLeft)
			{
				m_distance = 1600.f;

				Vector2 pos = GetMonster()->GetPos();
				pos.x += -m_dir * m_speed * 2 * DELTATIME;
				m_calDistance += m_speed * 2 * DELTATIME;
				GetMonster()->GetAnimator()->PlayAnim(L"Sweep", false);
				GetMonster()->SetPos(pos);
				GetMonster()->CreateHitBox(Vector2(10.f, -100.f), Vector2(150.f, 150.f));

				Camera::GetInst()->AddCamEffect(CAM_EFFECT::VIBRATION, 4.f);
				Camera::GetInst()->SetVibration(Vector2(sqrtf(2.f), sqrtf(2.f)), 4.f);

				if (!m_bSoundPlay)
				{
					PlaySoundEffect(L"Sweep", L"Sound\\ElderEnt_Sweeping.wav", true, 0.f, 60.f);
					m_bSoundPlay = true;
				}
			}
			else if (GetMonster()->GetName() == L"RightHand" && !((Boss*)GetMonster())->IsLeft() && m_bRight)
			{
				m_distance = 1600.f;

				Vector2 pos = GetMonster()->GetPos();
				pos.x += -m_dir * m_speed * 2 * DELTATIME;
				m_calDistance += m_speed * 2 * DELTATIME;
				GetMonster()->GetAnimator()->PlayAnim(L"Sweep", false);
				GetMonster()->SetPos(pos);
				GetMonster()->CreateHitBox(Vector2(10.f, -100.f), Vector2(150.f, 150.f));

				Camera::GetInst()->AddCamEffect(CAM_EFFECT::VIBRATION, 4.f);
				Camera::GetInst()->SetVibration(Vector2(sqrtf(2.f), sqrtf(2.f)), 4.f);

				if (!m_bSoundPlay)
				{
					PlaySoundEffect(L"Sweep", L"Sound\\ElderEnt_Sweeping.wav", true, 0.f, 60.f);
					m_bSoundPlay = true;
				}
			}

			if (m_calDistance >= m_distance)
			{
				Camera::GetInst()->StopVibration();
				m_calDistance = 0.f;
				--m_count;
				GetMonster()->SetPos(m_startPos);
				m_bLeft = false;
				m_bRight = false;
				m_bSoundPlay = false;
				StopSound(L"Sweep");

				((Boss*)GetMonster())->SweepHandSelect();

				if (((Boss*)GetMonster())->IsLeft())
				{
					m_bLeft = true;
				}
				else
				{
					m_bRight = true;
				}
			}
		}
		else
		{
			HitBox* hitBox = GetMonster()->GetHitBox();
			if (nullptr != hitBox && !hitBox->IsDead())
			{
				DeleteObject(hitBox);
				GetMonster()->InitHitBox();
			}

			Vector2 pos = GetMonster()->GetPos();
			Vector2 dir = m_prevPos - pos;
			float xDir = abs(dir.x);

			if (!dir.IsZero())
				dir.normalize();

			if (xDir > 3.2f)
			{
				pos.x += dir.x * m_speed * DELTATIME;
				GetMonster()->SetPos(pos);
			}
			else
			{
				if (GetMonster()->GetName() == L"LeftHand")
					g_bLeftHandFinish = true;

				if (GetMonster()->GetName() == L"RightHand")
					g_bRightHandFinish = true;
			}

			if(g_bLeftHandFinish && g_bRightHandFinish)
				ChangeState(L"Idle");
		}
	}
}

void State_Sweep::lateUpdate()
{
}

void State_Sweep::Enter()
{
	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}
	g_bLeftHandFinish = false;
	g_bRightHandFinish = false;
}

void State_Sweep::Exit()
{
	HitBox* hitBox = GetMonster()->GetHitBox();
	if (nullptr != hitBox)
	{
		DeleteObject(hitBox);
		GetMonster()->InitHitBox();
	}

	if (GetMonster()->GetName() == L"LeftHand" || GetMonster()->GetName() == L"RightHand")
	{
		GetMonster()->GetAnimator()->GetCurAnim()->Reset();
		GetMonster()->SetPos(m_prevPos);
	}
	m_bReady = false;
	m_bInit = true;
	m_calDistance = 0.f;
	m_distance = 500.f;
}
