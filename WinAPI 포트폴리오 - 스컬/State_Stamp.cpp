#include "stdafx.h"
#include "State_Stamp.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Boss.h"
#include "Scene.h"
#include "HitBox.h"
#include "Animator.h"
#include "Effect.h"
#include "Camera.h"

int State_Stamp::m_count = 3;
bool State_Stamp::g_bFinish = false;
bool State_Stamp::g_bLeftHandFinish = false;
bool State_Stamp::g_bRightHandFinish = false;

State_Stamp::State_Stamp() :
	m_target(nullptr),
	m_bReady(false),
	m_bInit(true),
	m_bGround(false),
	m_bSound(false),
	m_bRecoverySound(false),
	m_distance(200.f),
	m_calDistance(0.f),
	m_speed(200.f)
{
}

State_Stamp::~State_Stamp()
{
}

void State_Stamp::update()
{
	if (m_bInit)
	{
		m_prevPos = GetMonster()->GetPos();
		((Boss*)GetMonster())->SetPrevPos(m_prevPos);
		m_count = 3;
	}

	Vector2 pos = GetMonster()->GetPos();

	if (!m_bReady)
	{
		m_bInit = false;

		if (GetMonster()->GetName() == L"LeftHand" || GetMonster()->GetName() == L"RightHand")
		{
			pos.y -= m_speed * DELTATIME;
			m_calDistance += m_speed * DELTATIME;
			GetMonster()->GetAnimator()->PlayAnim(L"Strike", true);

			GetMonster()->SetPos(pos);

			if (m_calDistance >= m_distance)
			{
				m_bReady = true;
				m_calDistance = 0.f;
				m_startPos = pos;
				m_stampDir = m_target->GetPos() - pos;

				if (abs(m_stampDir.y) < 100.f)
				{
					m_stampDir.y += 100.f;
				}
				m_stampDir.normalize();
			}
		}
		else
		{
			m_bReady = true;
		}

		((Boss*)GetMonster())->StampHandSelect();
	}
	else
	{
		if (m_count > 0)
		{
			if (!m_bGround)
			{
				if (1 == ((Boss*)GetMonster())->GetSelectedHand() && GetMonster()->GetName() == L"LeftHand")
				{
					pos.x += m_stampDir.x * m_speed * DELTATIME;
					pos.y += abs(m_stampDir.y) * m_speed * DELTATIME;
					GetMonster()->SetPos(pos);
					GetMonster()->GetAnimator()->PlayAnim(L"Strike", true);
					GetMonster()->CreateHitBox(Vector2(10.f, -110.f), Vector2(150.f, 150.f));
				}
				else if (2 == ((Boss*)GetMonster())->GetSelectedHand() && GetMonster()->GetName() == L"RightHand")
				{
					pos.x += m_stampDir.x * m_speed * DELTATIME;
					pos.y += abs(m_stampDir.y) * m_speed * DELTATIME;
					GetMonster()->SetPos(pos);
					GetMonster()->GetAnimator()->PlayAnim(L"Strike", true);
					GetMonster()->CreateHitBox(Vector2(10.f, -110.f), Vector2(150.f, 150.f));
				}

				if (((Boss*)GetMonster())->IsGround())
				{
					--m_count;
					m_bGround = true;
					if (!m_bSound)
					{
						PlaySoundEffect(L"FistSlam", L"Sound\\ElderEnt_FistSlam.wav", false, 0.f, 80.f);
						m_bSound = true;
					}
					((Boss*)GetMonster())->SetGround(false);

					Effect* effect = new Effect;
					effect->CreateAnimator();
					effect->GetAnimator()->Load(L"Data\\Anim\\Stamp_Effect.anim");
					effect->GetAnimator()->PlayAnim(L"Idle", false);
					effect->SetPos(pos);
					effect->SetMagnification(Vector2(2.f, 2.f));
					CreateObject(effect, GROUP_TYPE::EFFECT);

					HitBox* hitBox = GetMonster()->GetHitBox();

					if (nullptr != hitBox)
					{
						DeleteObject(hitBox);
						GetMonster()->InitHitBox();
					}
				}
			}
			else
			{
				Vector2 dir = m_startPos - pos;
				Vector2 distance = Vector2(abs(dir.x), abs(dir.y));
				
				if (!dir.IsZero())
				{
					dir.normalize();

					if (distance.x > 2.f && distance.y > 2.f)
					{
						pos.x += dir.x * m_speed * DELTATIME;
						pos.y += dir.y * m_speed * DELTATIME;
						GetMonster()->SetPos(pos);
					}
					else
					{
						if (!m_bRecoverySound)
						{
							PlaySoundEffect(L"FistSlamRecovery", L"Sound\\ElderEnt_FistSlam_Recovery.wav", false, 0.f, 80.f);
							m_bRecoverySound = true;
						}
						m_bRecoverySound = false;
						m_bGround = false;
						m_bSound = false;
						m_stampDir = m_target->GetPos() - pos;
						m_stampDir.normalize();
						((Boss*)GetMonster())->GetParentPart()->StampHandSelect();
					}
				}
				else
				{
					m_bGround = false;
					m_bSound = false;
					m_stampDir = m_target->GetPos() - pos;
					m_stampDir.normalize();
					((Boss*)GetMonster())->GetParentPart()->StampHandSelect();
				}
			}
		}
		else
		{
			if(m_bGround)
			{
				Vector2 dir = m_startPos - pos;
				Vector2 distance = Vector2(abs(dir.x), abs(dir.y));

				if (!dir.IsZero())
					dir.normalize();

				if (distance.x > 2.f && distance.y > 2.f)
				{
					pos.x += dir.x * m_speed * DELTATIME;
					pos.y += dir.y * m_speed * DELTATIME;
					GetMonster()->SetPos(pos);
				}
				else
				{
					if (!m_bRecoverySound)
					{
						PlaySoundEffect(L"FistSlamRecovery", L"Sound\\ElderEnt_FistSlam_Recovery.wav", false, 0.f, 80.f);
						m_bRecoverySound = true;
					}

					m_bGround = false;
					g_bFinish = true;
					m_bSound = false;
					m_bRecoverySound = false;
				}
			}
			
			if(g_bFinish)
			{
				Vector2 dir = m_prevPos - pos;
				float yDistance = abs(dir.y);

				if (!dir.IsZero())
					dir.normalize();

				if (yDistance > 3.5f)
				{
					pos.y += dir.y * m_speed * DELTATIME;
					GetMonster()->SetPos(pos);
				}
				else
				{
					if (GetMonster()->GetName() == L"LeftHand")
						g_bLeftHandFinish = true;

					if (GetMonster()->GetName() == L"RightHand")
						g_bRightHandFinish = true;
				}

				if (g_bLeftHandFinish && g_bRightHandFinish)
					ChangeState(L"Idle");
			}

		}
	}
}

void State_Stamp::lateUpdate()
{
}

void State_Stamp::Enter()
{
	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}

	g_bFinish = false;
	g_bLeftHandFinish = false;
	g_bRightHandFinish = false;
}

void State_Stamp::Exit()
{
	HitBox* hitBox = GetMonster()->GetHitBox();
	if (nullptr != hitBox)
	{
		DeleteObject(hitBox);
		GetMonster()->InitHitBox();
	}

	m_calDistance = 0.f;
	m_bReady = false;
	m_bInit = true;
	m_bGround = false;
}
