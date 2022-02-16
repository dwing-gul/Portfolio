#include "pch.h"
#include "SkeletonArbalistScript.h"
#include "PlayerScript.h"
#include <Engine/Animator2D.h>
#include <Engine/Transform.h>
#include <Engine/GameObject.h>
#include <Engine/SceneManager.h>

SkeletonArbalistScript::SkeletonArbalistScript() :
	MonsterScript(SCRIPT_TYPE::SKELETONARBALISTSCRIPT)
{
	SetMaxHP(16);
	SetDodge(5);
	SetProtection(0);
	SetSpeed(5);
	SetMonsterType(MONSTER_TYPE::PROFANE);
	SetStunResistance(10);
	SetMoveResistance(25);
	SetBleedResistance(200);
	SetBlightResistance(10);
	SetDebuffResistance(15);
}

SkeletonArbalistScript::~SkeletonArbalistScript()
{
}

void SkeletonArbalistScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\skeleton_arbalist\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_arbalist\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_arbalist\\Attack_Bayonet.anim");
	GetAnimator2D()->LoadAnim(L"anim\\skeleton_arbalist\\Attack_Crossbow.anim");

	GetAnimator2D()->Play(L"Idle");
}

void SkeletonArbalistScript::update()
{
	MonsterScript::update();
}

void SkeletonArbalistScript::lateUpdate()
{
	MonsterScript::lateUpdate();

	if (!IsAnimStart())
	{
		GameObject* arrowEffect = SceneManager::GetInst()->FindObjByName(L"ArrowShotEffect");
		if (nullptr != arrowEffect && !(arrowEffect->IsDead()))
		{
			DeleteObject(arrowEffect);
		}
	}
}

void SkeletonArbalistScript::SkillTrigger(GameObject* _target)
{
	srand((UINT)time(NULL));

	int channel = 0;

	switch (GetSkillType())
	{
	case SKILL_TYPE_MONSTER::QURREL:
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

		if (0 == position || 1 == position)
		{
			SetSkillType(SKILL_TYPE_MONSTER::BAYONET_JAB);

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

		GetAnimator2D()->Play(L"Attack_Crossbow");

		if (0 != targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 5 + 3) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 12;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(7 * 1.5f * 1.25f);

					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
				}
				else
				{
					damage = int(damage * 1.25f);

					// 적에게 공격 적중
					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
				}

				Ptr<Sound> qurrel = ResourceManager::GetInst()->Load<Sound>(L"Sound_Qurrel", L"sound\\skeleton_arbalist\\char_en_sklar_crossbowshot.wav");
				channel = qurrel->Play(1);
				qurrel->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> qurrel_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Qurrel_Miss", L"sound\\skeleton_arbalist\\char_en_sklar_crossbowshot_miss.wav");
				channel = qurrel_miss->Play(1);
				qurrel_miss->SetVolume(0.5f, channel);
			}
		}
		else
		{
			return;
		}

		Ptr<Prefab> arrow_shot = ResourceManager::GetInst()->Load<Prefab>(L"ArrowShotEffect", L"prefab\\ArrowShotEffect.prefab");
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 pos = camera->GetTransform()->GetLocalPos();
		pos.x -= 150.f;
		pos.y += 40.f;
		Instantiate(arrow_shot, pos, 2);

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::BAYONET_JAB:
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

		GetAnimator2D()->Play(L"Attack_Bayonet");

		if (0 == targetPosition || 1 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 3 + 2) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 2;

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

		Ptr<Sound> bayonet_jab = ResourceManager::GetInst()->Load<Sound>(L"Sound_Bayonet_Jab", L"sound\\skeleton_arbalist\\char_en_sklar_bayonet_jab.wav");
		channel = bayonet_jab->Play(1);
		bayonet_jab->SetVolume(0.5f, channel);

		SetAnimStart(true);
	}
		break;
	}

	SetMove(true);
}

void SkeletonArbalistScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));
	SKILL_TYPE_MONSTER type = SKILL_TYPE_MONSTER::END;

	UINT targetSelect = rand() % 101;

	if (GetPosition() == 2 || GetPosition() == 3)
	{
		type = SKILL_TYPE_MONSTER::QURREL;
	}
	else
	{
		type = SKILL_TYPE_MONSTER::BAYONET_JAB;
	}

	SetSkillType(type);

	if (type == SKILL_TYPE_MONSTER::QURREL)
	{
		if (_playerParty[1] == nullptr && _playerParty[2] == nullptr && _playerParty[3] == nullptr)
			SelectTarget(_playerParty);

		if (targetSelect < 34)
		{
			SetTarget(_playerParty[1]);
		}
		else if (targetSelect >= 34 && targetSelect < 37)
		{
			if(nullptr != _playerParty[2])
				SetTarget(_playerParty[2]);
			else
				SetTarget(_playerParty[1]);
		}
		else if (targetSelect < 67)
		{
			if (nullptr != _playerParty[3])
				SetTarget(_playerParty[3]);
			else
			{
				targetSelect = rand() % 101;

				if (targetSelect < 51)
				{
					SetTarget(_playerParty[1]);
				}
				else
				{
					if (_playerParty[2] != nullptr)
						SetTarget(_playerParty[2]);
					else
						SetTarget(_playerParty[1]);
				}
			}
		}
	}
	else if (type == SKILL_TYPE_MONSTER::BAYONET_JAB)
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
