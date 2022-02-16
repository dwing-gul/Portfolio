#include "pch.h"
#include "SkeletonCommonScript.h"
#include "PlayerScript.h"
#include <Engine/Animator2D.h>

SkeletonCommonScript::SkeletonCommonScript() :
	MonsterScript(SCRIPT_TYPE::SKELETONCOMMONSCRIPT)
{
	SetMaxHP(8);
	SetDodge(0);
	SetProtection(0);
	SetSpeed(1);
	SetMonsterType(MONSTER_TYPE::PROFANE);
	SetStunResistance(10);
	SetMoveResistance(10);
	SetBleedResistance(200);
	SetBlightResistance(10);
	SetDebuffResistance(15);
}

SkeletonCommonScript::~SkeletonCommonScript()
{
}

void SkeletonCommonScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\skeleton_common\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_common\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_common\\Attack.anim");

	GetAnimator2D()->Play(L"Idle");
}

void SkeletonCommonScript::update()
{
	MonsterScript::update();
}

void SkeletonCommonScript::lateUpdate()
{
	MonsterScript::lateUpdate();

}

void SkeletonCommonScript::SkillTrigger(GameObject* _target)
{
	srand((UINT)time(NULL));
	
	int channel = 0;
	Ptr<Sound> skel_com_attack = ResourceManager::GetInst()->Load<Sound>(L"Sound_Skel_Com_Attack", L"sound\\skeleton_common\\char_en_sklcom_cudgel.wav");
	Ptr<Sound> skel_com_attack_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Skel_Com_Attack_Miss", L"sound\\skeleton_common\\char_en_sklcom_cudgel_miss.wav");

	switch (GetSkillType())
	{
	case SKILL_TYPE_MONSTER::BUMP_IN_THE_NIGHT:
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

		if (3 == position)
			return;

		GetAnimator2D()->Play(L"Attack");

		if (0 == targetPosition || 1 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 4 + 2) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 2;

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

				channel = skel_com_attack->Play(1);
				skel_com_attack->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);

				channel = skel_com_attack_miss->Play(1);
				skel_com_attack_miss->SetVolume(0.5f, channel);
			}
		}
		else
		{
			return;
		}

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::TIC_TOC:
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

		if (3 != position)
			return;

		GetAnimator2D()->Play(L"Attack");

		if (0 == targetPosition || 1 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 4 + 2) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));

			if ((int)percentage < totalAcc)
			{
				// 적에게 공격 적중
				((PlayerScript*)vectorTargetScript[0])->Hit(damage);

				channel = skel_com_attack->Play(1);
				skel_com_attack->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);

				channel = skel_com_attack_miss->Play(1);
				skel_com_attack_miss->SetVolume(0.5f, channel);
			}
		}
		else
		{
			return;
		}

		SetAnimStart(true);
	}
		break;
	}

	SetMove(true);
}

void SkeletonCommonScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));
	SKILL_TYPE_MONSTER type = SKILL_TYPE_MONSTER::END;

	UINT targetSelect = rand() % 101;

	if (GetPosition() != 3)
	{
		type = SKILL_TYPE_MONSTER::BUMP_IN_THE_NIGHT;
	}
	else
	{
		type = SKILL_TYPE_MONSTER::TIC_TOC;
	}

	SetSkillType(type);

	if (targetSelect < 51)
	{
		SetTarget(_playerParty[0]);
	}
	else
	{
		if(nullptr != _playerParty[1])
			SetTarget(_playerParty[1]);
		else
			SetTarget(_playerParty[0]);
	}
}
