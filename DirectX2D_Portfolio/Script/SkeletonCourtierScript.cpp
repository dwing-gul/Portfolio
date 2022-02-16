#include "pch.h"
#include "SkeletonCourtierScript.h"
#include "PlayerScript.h"
#include <Engine/Animator2D.h>
#include <Engine/SceneManager.h>

SkeletonCourtierScript::SkeletonCourtierScript() :
	MonsterScript(SCRIPT_TYPE::SKELETONCOURTIERSCRIPT)
{
	SetMaxHP(10);
	SetDodge(13);
	SetProtection(0);
	SetSpeed(8);
	SetMonsterType(MONSTER_TYPE::PROFANE);
	SetStunResistance(10);
	SetMoveResistance(10);
	SetBleedResistance(200);
	SetBlightResistance(10);
	SetDebuffResistance(15);
}

SkeletonCourtierScript::~SkeletonCourtierScript()
{
}

void SkeletonCourtierScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\skeleton_courtier\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_courtier\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_courtier\\Attack_Dagger.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_courtier\\Attack_Goblet.anim");

	GetAnimator2D()->Play(L"Idle");
}

void SkeletonCourtierScript::update()
{
	MonsterScript::update();
}

void SkeletonCourtierScript::lateUpdate()
{
	MonsterScript::lateUpdate();
}

void SkeletonCourtierScript::SkillTrigger(GameObject* _target)
{
	srand((UINT)time(NULL));

	int channel = 0;
	
	switch (GetSkillType())
	{
	case SKILL_TYPE_MONSTER::TEMPTING_GOBLET:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int targetDodge = ((PlayerScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 93;
		int totalAcc = baseAcc - targetDodge;

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;

		if (0 == position)
		{
			SetSkillType(SKILL_TYPE_MONSTER::KNIFE_IN_THE_DARK);

			if (2 == targetPosition || 3 == targetPosition)
			{
				UINT reSelect = rand() % 101;
				vector<GameObject*> vectorTarget;
				const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
				for (size_t i = 0; i < vectorPlayer.size(); ++i)
				{
					if (((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->GetPosition() == 0 || ((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->GetPosition() == 1)
					{
						vectorTarget.push_back(vectorPlayer[i]);
					}
				}

				if (reSelect < 51)
				{
					SkillTrigger(vectorTarget[0]);
				}
				else
				{
					SkillTrigger(vectorTarget[1]);
				}
			}
			return;
		}

		GetAnimator2D()->Play(L"Attack_Goblet");

		percentage = rand() % 101;
		int damage = int(float(rand() % 3 + 2) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));

		if ((int)percentage < totalAcc)
		{
			// 적에게 공격 적중
			((PlayerScript*)vectorTargetScript[0])->Hit(damage);
			((PlayerScript*)vectorTargetScript[0])->AddStress(15);

			Ptr<Sound> tempting_goblet = ResourceManager::GetInst()->Load<Sound>(L"Sound_Tempting_Goblet", L"sound\\skeleton_courtier\\char_en_sklco_temptinggob.wav");
			channel = tempting_goblet->Play(1);
			tempting_goblet->SetVolume(0.5f, channel);
		}
		else
		{
			// 적이 공격 회피
			((PlayerScript*)vectorTargetScript[0])->Dodge(true);

			Ptr<Sound> tempting_goblet_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Tempting_Goblet_Miss", L"sound\\skeleton_courtier\\char_en_sklco_temptinggob_miss.wav");
			channel = tempting_goblet_miss->Play(1);
			tempting_goblet_miss->SetVolume(0.5f, channel);
		}

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::KNIFE_IN_THE_DARK:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int targetDodge = ((PlayerScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 63;
		int totalAcc = baseAcc - targetDodge;

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;

		if (3 == position || 2 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Dagger");

		if (3 != targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 3 + 2) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 6;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(4 * 1.5f);

					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
				}
				else
				{
					// 적에게 공격 적중
					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
				}
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

		Ptr<Sound> knife_in_the_dark = ResourceManager::GetInst()->Load<Sound>(L"Sound_Knife_In_The_Dark", L"sound\\skeleton_courtier\\char_en_sklco_daggerjab.wav");
		channel = knife_in_the_dark->Play(1);
		knife_in_the_dark->SetVolume(0.5f, channel);

		SetAnimStart(true);
	}
		break;
	}

	SetMove(true);
}

void SkeletonCourtierScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));
	SKILL_TYPE_MONSTER type = SKILL_TYPE_MONSTER::END;

	UINT targetSelect = rand() % 101;
	UINT skillSelect = rand() % 101;

	if (GetPosition() == 0)
	{
		type = SKILL_TYPE_MONSTER::KNIFE_IN_THE_DARK;
	}
	else if(GetPosition() == 1)
	{
		if (skillSelect < 51)
		{
			type = SKILL_TYPE_MONSTER::TEMPTING_GOBLET;
		}
		else
		{
			type = SKILL_TYPE_MONSTER::KNIFE_IN_THE_DARK;
		}
	}
	else
	{
		type = SKILL_TYPE_MONSTER::TEMPTING_GOBLET;
	}

	SetSkillType(type);

	if (type == SKILL_TYPE_MONSTER::TEMPTING_GOBLET)
	{
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
	else if (type == SKILL_TYPE_MONSTER::KNIFE_IN_THE_DARK)
	{
		if (targetSelect < 51)
		{
			SetTarget(_playerParty[0]);
		}
		else
		{
			if (nullptr != _playerParty[1])
				SetTarget(_playerParty[1]);
			else
				SetTarget(_playerParty[0]);
		}
	}
}
