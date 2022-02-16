#include "pch.h"
#include "PewMediumScript.h"
#include <Engine/Animator2D.h>

PewMediumScript::PewMediumScript() :
	MonsterScript(SCRIPT_TYPE::PEWMEDIUMSCRIPT),
	m_bDie(false)
{
	SetMaxHP(40);
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

PewMediumScript::~PewMediumScript()
{
}

void PewMediumScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\pew_medium\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\pew_medium\\Defend.anim");

	GetAnimator2D()->Play(L"Idle");
}

void PewMediumScript::update()
{
	if (IsHit())
	{
		Ptr<Sound> pew_medium_hit = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pew_Medium_Hit", L"sound\\pew\\char_en_vo_pew_hurt_02.wav");
		pew_medium_hit->Play(1, 0.5f);
	}

	MonsterScript::update();
}

void PewMediumScript::lateUpdate()
{
	MonsterScript::lateUpdate();

	if (GetCurHP() <= 0)
	{
		if (!m_bDie)
		{
			m_bDie = true;
			Ptr<Sound> pew_medium_die = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pew_Medium_Die", L"sound\\pew\\char_en_vo_pew_death_02.wav");
			pew_medium_die->Play(1, 0.5f);
		}
	}
}

void PewMediumScript::SkillTrigger(GameObject* _target)
{
	SetTurnEnd(true);
}

void PewMediumScript::SelectTarget(const vector<GameObject*>& _playerParty)
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
