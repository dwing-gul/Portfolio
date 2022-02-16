#include "pch.h"
#include "PewLargeScript.h"
#include <Engine/Animator2D.h>

PewLargeScript::PewLargeScript() :
	MonsterScript(SCRIPT_TYPE::PEWLARGESCRIPT),
	m_bDie(false)
{
	SetMaxHP(55);
	SetDodge(0);
	SetProtection(0);
	SetSpeed(0);
	SetMonsterType(MONSTER_TYPE::WOODWORK);
	SetStunResistance(200);
	SetMoveResistance(300);
	SetBleedResistance(200);
	SetBlightResistance(200);
	SetDebuffResistance(200);
}

PewLargeScript::~PewLargeScript()
{
}

void PewLargeScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\pew_large\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\pew_large\\Defend.anim");

	GetAnimator2D()->Play(L"Idle");
}

void PewLargeScript::update()
{
	if (IsHit())
	{
		Ptr<Sound> pew_large_hit = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pew_Large_Hit", L"sound\\pew\\char_en_vo_pew_hurt_03.wav");
		pew_large_hit->Play(1, 0.5f);
	}

	MonsterScript::update();
}

void PewLargeScript::lateUpdate()
{
	MonsterScript::lateUpdate();

	if (GetCurHP() <= 0)
	{
		if (!m_bDie)
		{
			m_bDie = true;
			Ptr<Sound> pew_large_die = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pew_Large_Die", L"sound\\pew\\char_en_vo_pew_death_03.wav");
			pew_large_die->Play(1, 0.5f);
		}
	}
}

void PewLargeScript::SkillTrigger(GameObject* _target)
{
	SetTurnEnd(true);
}

void PewLargeScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));
	UINT targetSelect = rand() % 101;

	if (targetSelect < 26)
	{
		SetTarget(_playerParty[0]);
	}
	else if (targetSelect >= 26 && targetSelect < 51)
	{
		if (_playerParty[1] != nullptr)
			SetTarget(_playerParty[1]);
		else
			SetTarget(_playerParty[0]);
	}
	else if (targetSelect >= 51 && targetSelect < 76)
	{
		if (_playerParty[2] != nullptr)
			SetTarget(_playerParty[2]);
		else
		{
			targetSelect = rand() % 101;

			if (targetSelect < 51)
			{
				SetTarget(_playerParty[0]);
			}
			else
			{
				if (_playerParty[1] != nullptr)
					SetTarget(_playerParty[1]);
				else
					SetTarget(_playerParty[0]);
			}
		}
	}
	else if (targetSelect >= 76)
	{
		if (_playerParty[3] != nullptr)
			SetTarget(_playerParty[3]);
		else
		{
			targetSelect = rand() % 101;

			if (targetSelect < 34)
			{
				SetTarget(_playerParty[0]);
			}
			else if (targetSelect >= 34 && targetSelect < 67)
			{
				if (_playerParty[1] != nullptr)
					SetTarget(_playerParty[1]);
				else
					SetTarget(_playerParty[0]);
			}
			else
			{
				if (_playerParty[2] != nullptr)
					SetTarget(_playerParty[2]);
				else
				{
					targetSelect = rand() % 101;

					if (targetSelect < 51)
					{
						SetTarget(_playerParty[0]);
					}
					else
					{
						if (_playerParty[1] != nullptr)
							SetTarget(_playerParty[1]);
						else
							SetTarget(_playerParty[0]);
					}
				}
			}
		}
	}
}
