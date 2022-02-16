#include "pch.h"
#include "CultistWitchScript.h"
#include "PlayerScript.h"
#include "LightScript.h"
#include <Engine/Animator2D.h>
#include <Engine/SceneManager.h>

CultistWitchScript::CultistWitchScript() :
	MonsterScript(SCRIPT_TYPE::CULTISTWITCHSCRIPT)
{
	SetMaxHP(13);
	SetDodge(13);
	SetProtection(0);
	SetSpeed(7);
	SetMonsterType(MONSTER_TYPE::HUMAN);
	SetStunResistance(25);
	SetMoveResistance(10);
	SetBleedResistance(20);
	SetBlightResistance(20);
	SetDebuffResistance(40);
}

CultistWitchScript::~CultistWitchScript()
{
}

void CultistWitchScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\cultist_witch\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\cultist_witch\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\cultist_witch\\Attack.anim");

	GetAnimator2D()->Play(L"Idle");
}

void CultistWitchScript::update()
{
	MonsterScript::update();
}

void CultistWitchScript::lateUpdate()
{
	MonsterScript::lateUpdate();
}

void CultistWitchScript::SkillTrigger(GameObject* _target)
{
	srand((UINT)time(NULL));

	int channel = 0;

	switch (GetSkillType())
	{
	case SKILL_TYPE_MONSTER::STRESSFUL_INCANTATION:
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

		GetAnimator2D()->Play(L"Attack");

		percentage = rand() % 101;
		int damage = int(1.f * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));

		if ((int)percentage < totalAcc)
		{
			// 적에게 공격 적중
			((PlayerScript*)vectorTargetScript[0])->Hit(damage);
			((PlayerScript*)vectorTargetScript[0])->AddStress(15);

			// 횃불 밝기 줄임
			GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
			const vector<Script*>& vectorLightScript = light->GetScripts();
			((LightScript*)vectorLightScript[0])->SubLight(10.f);
		}
		else
		{
			// 적이 공격 회피
			((PlayerScript*)vectorTargetScript[0])->Dodge(true);
		}

		Ptr<Sound> stressful_incansation = ResourceManager::GetInst()->Load<Sound>(L"Sound_Stressful_Incansation", L"sound\\cultist_witch\\char_en_cultist_f_incant.wav");
		channel = stressful_incansation->Play(1);
		stressful_incansation->SetVolume(0.5f, channel);

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::ELDRITCH_PULL:
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

		if (0 == position)
		{
			UINT reSelect = rand() % 101;

			if (reSelect < 51)
			{
				SetSkillType(SKILL_TYPE_MONSTER::STRESSFUL_INCANTATION);
			}
			else
			{
				SetSkillType(SKILL_TYPE_MONSTER::ELDRITCH_PUSH);
			}

			if (GetSkillType() == SKILL_TYPE_MONSTER::ELDRITCH_PUSH)
			{
				reSelect = rand() % 101;
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
			else
				SkillTrigger(_target);

			return;
		}

		GetAnimator2D()->Play(L"Attack");

		if (2 == targetPosition || 3 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(1.f * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 6;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(1 * 1.5f);

					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
					((PlayerScript*)vectorTargetScript[0])->AddStress(5);
				}
				else
				{
					// 적에게 공격 적중
					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
					((PlayerScript*)vectorTargetScript[0])->AddStress(5);
				}

				Ptr<Sound> eldritch_pull = ResourceManager::GetInst()->Load<Sound>(L"Sound_Eldritch_Pull", L"sound\\cultist_witch\\char_en_cultist_f_pull.wav");
				channel = eldritch_pull->Play(1);
				eldritch_pull->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> eldritch_pull_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Eldritch_Pull_Miss", L"sound\\cultist_witch\\char_en_cultist_f_pull_miss.wav");
				channel = eldritch_pull_miss->Play(1);
				eldritch_pull_miss->SetVolume(0.5f, channel);
			}
		}
		else
		{
			return;
		}

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::ELDRITCH_PUSH:
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

		GetAnimator2D()->Play(L"Attack");

		if (0 == targetPosition || 1 == targetPosition)
		{
			percentage = rand() % 101;
			int damage = int(1.f * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = 6;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(1 * 1.5f);

					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
					((PlayerScript*)vectorTargetScript[0])->AddStress(5);
				}
				else
				{
					// 적에게 공격 적중
					((PlayerScript*)vectorTargetScript[0])->Hit(damage);
					((PlayerScript*)vectorTargetScript[0])->AddStress(5);
				}

				Ptr<Sound> eldritch_push = ResourceManager::GetInst()->Load<Sound>(L"Sound_Eldritch_Push", L"sound\\cultist_witch\\char_en_cultist_f_push.wav");
				channel = eldritch_push->Play(1);
				eldritch_push->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((PlayerScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> eldritch_push_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Eldritch_Push_Miss", L"sound\\cultist_witch\\char_en_cultist_f_push_miss.wav");
				channel = eldritch_push_miss->Play(1);
				eldritch_push_miss->SetVolume(0.5f, channel);
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

void CultistWitchScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));
	SKILL_TYPE_MONSTER type = SKILL_TYPE_MONSTER::END;

	UINT targetSelect = rand() % 101;
	UINT skillSelect = rand() % 101;

	if (GetPosition() == 0)
	{
		if (skillSelect < 51)
		{
			type = SKILL_TYPE_MONSTER::STRESSFUL_INCANTATION;
		}
		else
		{
			type = SKILL_TYPE_MONSTER::ELDRITCH_PUSH;
		}
	}
	else
	{
		if (skillSelect < 34)
		{
			type = SKILL_TYPE_MONSTER::STRESSFUL_INCANTATION;
		}
		else if (skillSelect >= 34 && skillSelect < 67)
		{
			type = SKILL_TYPE_MONSTER::ELDRITCH_PULL;
		}
		else if (skillSelect >= 67)
		{
			type = SKILL_TYPE_MONSTER::ELDRITCH_PUSH;
		}
	}

	SetSkillType(type);

	if (type == SKILL_TYPE_MONSTER::STRESSFUL_INCANTATION)
	{
		if (targetSelect < 26)
		{
			SetTarget(_playerParty[0]);
		}
		else if (targetSelect >= 26 && targetSelect < 51)
		{
			if(_playerParty[1] != nullptr)
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
	else if (type == SKILL_TYPE_MONSTER::ELDRITCH_PULL)
	{
		if (_playerParty[2] == nullptr && _playerParty[3] == nullptr)
			SelectTarget(_playerParty);

		if (targetSelect < 51)
		{
			SetTarget(_playerParty[2]);
		}
		else
		{
			if (_playerParty[3] != nullptr)
				SetTarget(_playerParty[3]);
			else
				SetTarget(_playerParty[2]);
		}
	}
	else if (type == SKILL_TYPE_MONSTER::ELDRITCH_PUSH)
	{
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
