#include "pch.h"
#include "SkeletonDefenderScript.h"
#include "PlayerScript.h"
#include <Engine/Animator2D.h>

SkeletonDefenderScript::SkeletonDefenderScript() :
	MonsterScript(SCRIPT_TYPE::SKELETONDEFENDERSCRIPT)
{
	SetMaxHP(15);
	SetDodge(0);
	SetProtection(25);
	SetSpeed(0);
	SetMonsterType(MONSTER_TYPE::PROFANE);
	SetStunResistance(25);
	SetMoveResistance(50);
	SetBleedResistance(200);
	SetBlightResistance(10);
	SetDebuffResistance(15);
}

SkeletonDefenderScript::~SkeletonDefenderScript()
{
}

void SkeletonDefenderScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\skeleton_defender\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_defender\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_defender\\Attack_Axe.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_defender\\Attack_Shield.anim");

	GetAnimator2D()->Play(L"Idle");
}

void SkeletonDefenderScript::update()
{
	MonsterScript::update();
}

void SkeletonDefenderScript::lateUpdate()
{
	MonsterScript::lateUpdate();
}

void SkeletonDefenderScript::SkillTrigger(GameObject* _target)
{
	srand((UINT)time(NULL));

	int channel = 0;

	Ptr<Sound> axe_blade = ResourceManager::GetInst()->Load<Sound>(L"Sound_Axe_Blade", L"sound\\skeleton_defender\\char_en_sklde_axestrike.wav");
	
	switch (GetSkillType())
	{
	case SKILL_TYPE_MONSTER::AXEBLADE:
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

		if (2 == position || 3 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Axe");

		if (0 == targetPosition || 1 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 3 + 3) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 6;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(5 * 1.5f);

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

		channel = axe_blade->Play(1);
		axe_blade->SetVolume(0.5f, channel);

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::DEAD_WEIGHT:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int targetDodge = ((PlayerScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 83;
		int totalAcc = baseAcc - targetDodge;

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		UINT stunPercent = 100 - ((PlayerScript*)vectorTargetScript[0])->GetStunResistance();

		if (2 == position || 3 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Shield");

		if (0 == targetPosition || 1 == targetPosition)
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

					percentage = rand() % 101;
					if (percentage < stunPercent)
						((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::STUN, 1);
				}
				else
				{
					// 적에게 공격 적중
					((PlayerScript*)vectorTargetScript[0])->Hit(damage);

					percentage = rand() % 101;
					if (percentage < stunPercent)
						((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::STUN, 1);
				}

				Ptr<Sound> dead_weight = ResourceManager::GetInst()->Load<Sound>(L"Sound_Dead_Weight", L"sound\\skeleton_defender\\char_en_sklde_shieldbash.wav");
				channel = dead_weight->Play(1);
				dead_weight->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> dead_weight_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Dead_Weight_Miss", L"sound\\skeleton_defender\\char_en_sklde_shieldbash_miss.wav");
				channel = dead_weight_miss->Play(1);
				dead_weight_miss->SetVolume(0.5f, channel);
			}
		}
		else
		{
			return;
		}

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::CLUMSY_AXEBLADE:
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

		if (3 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Axe");

		if (3 != targetPosition)
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

		channel = axe_blade->Play(1);
		axe_blade->SetVolume(0.5f, channel);

		SetAnimStart(true);
	}
		break;
	}

	SetMove(true);
}

void SkeletonDefenderScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));
	SKILL_TYPE_MONSTER type = SKILL_TYPE_MONSTER::END;

	UINT targetSelect = rand() % 101;
	UINT skillSelect = rand() % 101;

	if (GetPosition() == 0 || GetPosition() == 1)
	{
		if (skillSelect < 51)
		{
			type = SKILL_TYPE_MONSTER::AXEBLADE;
		}
		else
		{
			type = SKILL_TYPE_MONSTER::DEAD_WEIGHT;
		}
	}
	else
	{
		type = SKILL_TYPE_MONSTER::CLUMSY_AXEBLADE;
	}

	SetSkillType(type);

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
