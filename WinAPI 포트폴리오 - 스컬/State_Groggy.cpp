#include "stdafx.h"
#include "State_Groggy.h"
#include "Animator.h"
#include "Tile.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Effect.h"
#include "Boss.h"

bool State_Groggy::g_bGround = false;
bool State_Groggy::g_bFinish = false;

State_Groggy::State_Groggy() :
	m_bInit(true),
	m_bSound(false),
	m_bRecoverySound(false),
	m_timer(3.f),
	m_speed(200.f)
{
}

State_Groggy::~State_Groggy()
{
}

void State_Groggy::update()
{
	if (m_bInit)
	{
		m_prevPos = GetMonster()->GetPos();
		m_bInit = false;
	}

	if (!g_bGround)
	{
		vector<Obj*> tile = SceneManager::GetInst()->GetCurScene()->GetObjects(GROUP_TYPE::TILE);
		Vector2 pos = GetMonster()->GetPos();

		if (GetMonster()->GetName() == L"LeftHand" || GetMonster()->GetName() == L"RightHand")
		{
			for (size_t i = 0; i < tile.size(); ++i)
			{
				if (((Tile*)tile[i])->GetTileType() == TILE_TYPE::GROUND)
				{
					pos.y = tile[i]->GetPos().y;
					break;
				}
			}

			GetMonster()->SetPos(pos);
			GetMonster()->GetAnimator()->PlayAnim(L"Groggy", true);
		}
		else if (GetMonster()->GetName() == L"BossBody")
		{
			float targetPos = 0.f;
			Vector2 animPos = Vector2(pos.x, 0.f);
			for (size_t i = 0; i < tile.size(); ++i)
			{
				if (((Tile*)tile[i])->GetTileType() == TILE_TYPE::GROUND)
				{
					targetPos = tile[i]->GetPos().y + 165.f;
					animPos.y = tile[i]->GetPos().y;
					break;
				}
			}

			pos.y += m_speed * DELTATIME;

			if (pos.y >= targetPos)
			{
				pos.y = targetPos;
				g_bGround = true;

				Effect* effect = new Effect;
				effect->CreateAnimator();
				effect->GetAnimator()->Load(L"Data\\Anim\\Stamp_Effect.anim");
				effect->GetAnimator()->PlayAnim(L"Idle", false);
				effect->SetPos(animPos);
				effect->SetMagnification(Vector2(2.f, 2.f));
				CreateObject(effect, GROUP_TYPE::EFFECT);
			}

			GetMonster()->SetPos(pos);
			GetMonster()->GetAnimator()->PlayAnim(L"Idle", true);
		}
		else if (GetMonster()->GetName() == L"BossHead")
		{
			GetMonster()->GetAnimator()->PlayAnim(L"EndProjectile", true);
		}
		else 
		{
			GetMonster()->GetAnimator()->PlayAnim(L"Idle", true);
		}
	}

	if (GetMonster()->IsDie() && g_bGround)
	{
		if (GetMonster()->GetName() == L"BossBody" && !m_bSound)
		{
			PlaySoundEffect(L"GroggyImpact", L"Sound\\ElderEnt_Groggy_Impact.wav", false, 0.f, 70.f);
			m_bSound = true;
		}
	}
}

void State_Groggy::lateUpdate()
{
	if (!GetMonster()->IsDie() && g_bGround)
	{
		if (GetMonster()->GetName() == L"BossBody")
		{
			if (!m_bSound)
			{
				PlaySoundEffect(L"GroggyImpact", L"Sound\\ElderEnt_Groggy_Impact.wav", false, 0.f, 70.f);
				m_bSound = true;
			}

			if (m_timer <= 0.f)
			{
				Vector2 pos = GetMonster()->GetPos();
				Vector2 dir = m_prevPos - pos;
				float yDistance = abs(dir.y);

				if (!dir.IsZero())
					dir.normalize();

				if (yDistance > 3.f)
				{
					pos.y += dir.y * m_speed * DELTATIME;
					GetMonster()->SetPos(pos);

					if (!m_bRecoverySound)
					{
						PlaySoundEffect(L"GroggyRecovery", L"Sound\\ElderEnt_Groggy_Recovery.wav", false, 0.f, 70.f);
						m_bRecoverySound = true;
					}
				}
				else
				{
					g_bGround = false;
					g_bFinish = true;
					m_bRecoverySound = false;
					m_bSound = false;
				}
			}
			else
				m_timer -= DELTATIME;
		}

		if (g_bFinish)
		{
			g_bFinish = false;
			ChangeState(L"Idle");
		}
	}
}

void State_Groggy::Enter()
{
	if (GetMonster()->IsDie())
	{
		g_bGround = false;
		((Boss*)GetMonster())->SetGroggy(true);
	}
}

void State_Groggy::Exit()
{
	m_bInit = true;
	m_timer = 3.f;
	m_bSound = false;
}
