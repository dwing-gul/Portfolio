#include "pch.h"
#include "CultistBrawlerScript.h"
#include "PlayerScript.h"
#include <Engine/Animator2D.h>

CultistBrawlerScript::CultistBrawlerScript() :
	MonsterScript(SCRIPT_TYPE::CULTISTBRAWLERSCRIPT)
{
	SetMaxHP(15);
	SetDodge(0);
	SetProtection(0);
	SetSpeed(5);
	SetMonsterType(MONSTER_TYPE::HUMAN);
	SetStunResistance(25);
	SetMoveResistance(25);
	SetBleedResistance(20);
	SetBlightResistance(20);
	SetDebuffResistance(15);
}

CultistBrawlerScript::~CultistBrawlerScript()
{
}

void CultistBrawlerScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\cultist_brawler\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\cultist_brawler\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\cultist_brawler\\Attack_Melee.anim");

	GetAnimator2D()->Play(L"Idle");
}

void CultistBrawlerScript::update()
{
	MonsterScript::update();
}

void CultistBrawlerScript::lateUpdate()
{
	MonsterScript::lateUpdate();
}

void CultistBrawlerScript::SkillTrigger(GameObject* _target)
{
	srand((UINT)time(NULL));

	int channel = 0;

	switch (GetSkillType())
	{
	case SKILL_TYPE_MONSTER::REND_FOR_THE_OLD_GOD:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int targetDodge = ((PlayerScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 73;
		int totalAcc = baseAcc - targetDodge;

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		UINT bleedPercent = 100 - ((PlayerScript*)vectorTargetScript[0])->GetBleedResistance();
		UINT debuffPercent = 100 - ((PlayerScript*)vectorTargetScript[0])->GetDebuffResistance();

		if (2 == position || 3 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Melee");

		if (0 == targetPosition || 1 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 3 + 2) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 12;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(4 * 1.5f);

					((PlayerScript*)vectorTargetScript[0])->Hit(damage);

					percentage = rand() % 101;
					if (percentage < bleedPercent)
						((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::BLEED, 1, 1);

					percentage = rand() % 101;
					if (percentage < debuffPercent)
					{
						float stress = ((PlayerScript*)vectorTargetScript[0])->GetCurStress() * 0.1f;
						((PlayerScript*)vectorTargetScript[0])->AddStress((UINT)stress);
					}
				}
				else
				{
					// 적에게 공격 적중
					((PlayerScript*)vectorTargetScript[0])->Hit(damage);

					percentage = rand() % 101;
					if (percentage < bleedPercent)
						((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::BLEED, 1, 1);

					percentage = rand() % 101;
					if (percentage < debuffPercent)
					{
						float stress = ((PlayerScript*)vectorTargetScript[0])->GetCurStress() * 0.1f;
						((PlayerScript*)vectorTargetScript[0])->AddStress((UINT)stress);
					}
				}

				Ptr<Sound> rend_for_the_old_god = ResourceManager::GetInst()->Load<Sound>(L"Sound_Rend_For_The_Old_God", L"sound\\cultist_brawler\\char_en_cultist_warlord_claw.wav");
				channel = rend_for_the_old_god->Play(1);
				rend_for_the_old_god->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> rend_for_the_old_god_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Rend_For_The_Old_God_Miss", L"sound\\cultist_brawler\\char_en_cultist_warlord_claw_miss.wav");
				channel = rend_for_the_old_god_miss->Play(1);
				rend_for_the_old_god_miss->SetVolume(0.5f, channel);
			}
		}
		else
		{
			return;
		}

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::STUMBLING_SCRATCH:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int targetDodge = ((PlayerScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 43;
		int totalAcc = baseAcc - targetDodge;

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;

		if (2 == position || 3 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Melee");

		if (0 == targetPosition || 1 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 3 + 2) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));

			if ((int)percentage < totalAcc)
			{
				// 적에게 공격 적중
				((PlayerScript*)vectorTargetScript[0])->Hit(damage);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);
			}
		}
		else
		{
			return;
		}

		Ptr<Sound> stunbling_scratch = ResourceManager::GetInst()->Load<Sound>(L"Sound_Stumbling_Scratch", L"sound\\cultist_brawler\\char_en_cultist_m_claw.wav");
		channel = stunbling_scratch->Play(1);
		stunbling_scratch->SetVolume(0.5f, channel);

		SetAnimStart(true);
	}
		break;
	}

	SetMove(true);
}

void CultistBrawlerScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));

	SKILL_TYPE_MONSTER type = SKILL_TYPE_MONSTER::END;
	UINT targetSelect = rand() % 101;

	if (targetSelect < 51)
	{
		SetTarget(_playerParty[0]);
	}
	else
	{
		if(_playerParty[1] != nullptr)
			SetTarget(_playerParty[1]);
		else
			SetTarget(_playerParty[0]);
	}

	if (GetPosition() == 0 || GetPosition() == 1)
	{
		type = SKILL_TYPE_MONSTER::REND_FOR_THE_OLD_GOD;
	}
	else
	{
		type = SKILL_TYPE_MONSTER::STUMBLING_SCRATCH;
	}

	SetSkillType(type);
}
