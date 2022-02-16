#include "pch.h"
#include "PewSmallScript.h"
#include <Engine/Animator2D.h>

PewSmallScript::PewSmallScript() :
	MonsterScript(SCRIPT_TYPE::PEWSMALLSCRIPT),
	m_bDie(false)
{
	SetMaxHP(25);
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

PewSmallScript::~PewSmallScript()
{
}

void PewSmallScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\pew_small\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\pew_small\\Defend.anim");

	GetAnimator2D()->Play(L"Idle");
}

void PewSmallScript::update()
{
	if (IsHit())
	{
		Ptr<Sound> pew_small_hit = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pew_Small_Hit", L"sound\\pew\\char_en_vo_pew_hurt_01.wav");
		pew_small_hit->Play(1, 0.5f);
	}

	MonsterScript::update();
}

void PewSmallScript::lateUpdate()
{
	MonsterScript::lateUpdate();

	if (GetCurHP() <= 0)
	{
		if (!m_bDie)
		{
			m_bDie = true;
			Ptr<Sound> pew_small_die = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pew_Small_Die", L"sound\\pew\\char_en_vo_pew_death_01.wav");
			pew_small_die->Play(1, 0.5f);
		}
	}
}

void PewSmallScript::SkillTrigger(GameObject* _target)
{
	SetTurnEnd(true);
}

void PewSmallScript::SelectTarget(const vector<GameObject*>& _playerParty)
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
