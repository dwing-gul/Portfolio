#include "stdafx.h"
#include "Boss.h"
#include "AI.h"
#include "State_Idle.h"
#include "State_Sweep.h"
#include "State_Stamp.h"
#include "State_ShotMissile.h"
#include "State_Groggy.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "KeyManager.h"
#include "Animator.h"

Boss::Boss() :
	m_leftHand(nullptr),
	m_rightHand(nullptr),
	m_parentPart(nullptr),
	m_bLeft(false),
	m_bGround(false),
	m_bGroggy(false),
	m_prevPos(Vector2(0.f, 0.f)),
	m_handSelect(0)
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if (curScene->GetType() != SCENE_TYPE::TOOL)
	{
		CreateAI();
		GetAI()->AddState(L"Idle", new State_Idle);
		GetAI()->AddState(L"Sweep", new State_Sweep);
		GetAI()->AddState(L"ShotMissile", new State_ShotMissile);
		GetAI()->AddState(L"Groggy", new State_Groggy);
		GetAI()->AddState(L"Stamp", new State_Stamp);

		GetAI()->ChangeState(L"Idle");
	}
}

Boss::~Boss()
{
	if (!m_vectorBossBodyPart.empty())
	{
		for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
		{
			delete m_vectorBossBodyPart[i];
		}
		m_vectorBossBodyPart.clear();
	}
}

void Boss::update()
{
	m_HPRatio = float(m_curHP) / float(m_maxHP);

	if (m_parentPart)
	{
		wstring name = GetName();
		if (GetName() != L"LeftHand" && GetName() != L"RightHand")
		{
			Vector2 temp = m_parentPart->GetFinalPos();
			m_finalPos = GetPos() + m_parentPart->GetFinalPos();
		}
		else
			m_finalPos = GetPos();
	}
	else
	{
		m_finalPos = GetPos();

		if (m_bDie && !m_bGroggy)
		{
			if(GetAI()->GetCurStateName() == L"Sweep")
				StopSound(L"Sweep");

			if (GetAI()->GetCurStateName() == L"Stamp")
			{
				StopSound(L"FistSlam");
				StopSound(L"FistSlamRecovery");
			}

			StopSound(L"ShotReady");
			StopSound(L"Fire");

			Camera::GetInst()->StopVibration();
			SetPos(m_prevPos);
			for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
			{
				m_vectorBossBodyPart[i]->GetAI()->ChangeState(L"Groggy");
			}

			if (GetAI()->GetCurStateName() != L"ShotMissile" && GetAI()->GetCurStateName() != L"Groggy")
			{
				m_leftHand->SetPos(m_leftHand->m_prevPos);
				m_rightHand->SetPos(m_rightHand->m_prevPos);
			}
			m_leftHand->GetAI()->ChangeState(L"Groggy");
			m_rightHand->GetAI()->ChangeState(L"Groggy");

			GetAI()->ChangeState(L"Groggy");

			if (nullptr != GetCollider())
			{
				DeleteCollider();
			}
		}

		if (!m_bDie)
		{
			if (KEY_TAP(KEY::N1))
			{
				if (GetAI()->GetCurStateName() == L"Stamp")
				{
					StopSound(L"FistSlam");
					StopSound(L"FistSlamRecovery");
				}

				StopSound(L"ShotReady");
				StopSound(L"Fire");

				SetPos(m_prevPos);

				Camera::GetInst()->StopVibration();
				GetAnimator()->PlayAnim(L"Idle", true);
				for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
				{
					m_vectorBossBodyPart[i]->GetAI()->ChangeState(L"Sweep");
					m_vectorBossBodyPart[i]->GetAnimator()->PlayAnim(L"Idle", true);
				}

				if (GetAI()->GetCurStateName() != L"ShotMissile" && GetAI()->GetCurStateName() != L"Groggy")
				{
					m_leftHand->SetPos(m_leftHand->m_prevPos);
					m_rightHand->SetPos(m_rightHand->m_prevPos);
				}
				m_leftHand->GetAI()->ChangeState(L"Sweep");
				m_rightHand->GetAI()->ChangeState(L"Sweep");

				GetAI()->ChangeState(L"Sweep");
			}

			if (KEY_TAP(KEY::N2))
			{
				if (GetAI()->GetCurStateName() == L"Sweep")
					StopSound(L"Sweep");

				StopSound(L"ShotReady");
				StopSound(L"Fire");

				SetPos(m_prevPos);

				Camera::GetInst()->StopVibration();

				GetAnimator()->PlayAnim(L"Idle", true);
				for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
				{
					m_vectorBossBodyPart[i]->GetAI()->ChangeState(L"Stamp");
					m_vectorBossBodyPart[i]->GetAnimator()->PlayAnim(L"Idle", true);
				}

				if (GetAI()->GetCurStateName() != L"ShotMissile" && GetAI()->GetCurStateName() != L"Groggy")
				{
					m_leftHand->SetPos(m_leftHand->m_prevPos);
					m_rightHand->SetPos(m_rightHand->m_prevPos);
				}
				m_leftHand->GetAI()->ChangeState(L"Stamp");
				m_rightHand->GetAI()->ChangeState(L"Stamp");

				GetAI()->ChangeState(L"Stamp");
			}

			if (KEY_TAP(KEY::N3))
			{
				if (GetAI()->GetCurStateName() == L"Sweep")
					StopSound(L"Sweep");

				if (GetAI()->GetCurStateName() == L"Stamp")
				{
					StopSound(L"FistSlam");
					StopSound(L"FistSlamRecovery");
				}

				SetPos(m_prevPos);

				Camera::GetInst()->StopVibration();
				for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
				{
					m_vectorBossBodyPart[i]->GetAI()->ChangeState(L"ShotMissile");
				}

				if (GetAI()->GetCurStateName() != L"ShotMissile" && GetAI()->GetCurStateName() != L"Groggy")
				{
					m_leftHand->SetPos(m_leftHand->m_prevPos);
					m_rightHand->SetPos(m_rightHand->m_prevPos);
				}
				m_leftHand->GetAI()->ChangeState(L"ShotMissile");
				m_rightHand->GetAI()->ChangeState(L"ShotMissile");

				GetAI()->ChangeState(L"ShotMissile");
			}
		}
	}


	if (nullptr != m_ai)
		m_ai->update();

	if (m_bDie && nullptr != m_hitBox)
	{
		DeleteObject(m_hitBox);
		m_hitBox = nullptr;
	}

	if (!m_vectorBossBodyPart.empty())
	{
		for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
		{
			m_vectorBossBodyPart[i]->update();
		}
	}

	if (nullptr != m_leftHand)
	{
		m_leftHand->update();
	}

	if (nullptr != m_rightHand)
	{
		m_rightHand->update();
	}

	RigidBody::update();
}

void Boss::lateUpdate()
{
	if (nullptr != m_ai && !m_bDie)
		m_ai->lateUpdate();

	if (!m_vectorBossBodyPart.empty())
	{
		for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
		{
			m_vectorBossBodyPart[i]->lateUpdate();
		}
	}

	if (nullptr != m_leftHand)
	{
		m_leftHand->lateUpdate();
	}

	if (nullptr != m_rightHand)
	{
		m_rightHand->lateUpdate();
	}

	if (GetName() == L"BossHead")
	{
		int a = 0;
	}

	RigidBody::lateUpdate();
}

void Boss::render(HDC _dc)
{
	Monster::render(_dc);

	if (!m_vectorBossBodyPart.empty())
	{
		for (int i = 0; i < m_vectorBossBodyPart.size(); ++i)
		{
			m_vectorBossBodyPart[i]->render(_dc);
		}
	}

	if (nullptr != m_leftHand)
	{
		m_leftHand->render(_dc);
	}

	if (nullptr != m_rightHand)
	{
		m_rightHand->render(_dc);
	}
}

void Boss::SweepHandSelect()
{
	srand((unsigned int)time(0));

	float temp = (float)rand();
	float percent = rand() % 10000 / 100.f;

	if (nullptr == m_parentPart)
	{
		if (percent < 50.f)
			m_bLeft = true;
		else
			m_bLeft = false;
	}
	else
	{
		m_bLeft = m_parentPart->m_bLeft;
	}
}

void Boss::StampHandSelect()
{
	if (nullptr != m_leftHand && nullptr != m_rightHand)
	{
		Obj* player = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

		Vector2 leftHandDistance = m_leftHand->GetPos() - player->GetPos();
		Vector2 rightHandDistance = m_rightHand->GetPos() - player->GetPos();

		float leftDistance = leftHandDistance.length();
		float rightDistance = rightHandDistance.length();

		if (leftDistance < rightDistance)
		{
			m_handSelect = 1;
		}
		else
		{
			m_handSelect = 2;
		}

		m_leftHand->m_handSelect = m_handSelect;
		m_rightHand->m_handSelect = m_handSelect;
	}
}
