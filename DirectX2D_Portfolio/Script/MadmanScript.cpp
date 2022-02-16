#include "pch.h"
#include "MadmanScript.h"
#include "PlayerScript.h"
#include <Engine/Animator2D.h>

MadmanScript::MadmanScript() :
	MonsterScript(SCRIPT_TYPE::MADMANSCRIPT)
{
	SetMaxHP(105);
	SetDodge(5);
	SetProtection(0);
	SetSpeed(0);
	SetMonsterType(MONSTER_TYPE::PROFANE);
	SetStunResistance(100);
	SetMoveResistance(300);
	SetBleedResistance(0);
	SetBlightResistance(0);
	SetDebuffResistance(20);
}

MadmanScript::~MadmanScript()
{
}

void MadmanScript::start()
{
	MonsterScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\madman\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\madman\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\madman\\Attack_Point.anim");
	GetAnimator2D()->LoadAnim(L"anim\\madman\\Attack_Shout.anim");

	GetAnimator2D()->Play(L"Idle");
}

void MadmanScript::update()
{
	MonsterScript::update();
}

void MadmanScript::lateUpdate()
{
	MonsterScript::lateUpdate();
}

void MadmanScript::SkillTrigger(GameObject* _target)
{
	srand((UINT)time(NULL));

	int channel = 0;

	switch (GetSkillType())
	{
	case SKILL_TYPE_MONSTER::RUBBLE_OF_RUIN:
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

		GetAnimator2D()->Play(L"Attack_Point");

		percentage = rand() % 101;
		int damage = int(((rand() % 13) + 11) * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));

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

		Ptr<Sound> rubble = ResourceManager::GetInst()->Load<Sound>(L"Sound_Rubble", L"sound\\madman\\char_en_prophet_attack.wav");
		channel = rubble->Play(1);
		rubble->SetVolume(0.5f, channel);

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE_MONSTER::EYE_ON_YOU:
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
		UINT stunPercent = 100 - ((PlayerScript*)vectorTargetScript[0])->GetStunResistance();

		GetAnimator2D()->Play(L"Attack_Point");

		percentage = rand() % 101;
		int damage = int(1.f * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
		UINT critical = 2;

		if ((int)percentage < totalAcc)
		{
			if (percentage < critical)
			{
				// 크리티컬 데미지
				damage = int(1 * 1.5f);

				((PlayerScript*)vectorTargetScript[0])->Hit(damage);

				percentage = rand() % 101;
				if (percentage < stunPercent)
				{
					((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::STUN, 1);
					((PlayerScript*)vectorTargetScript[0])->AddStress(11);
				}
			}
			else
			{
				// 적에게 공격 적중
				((PlayerScript*)vectorTargetScript[0])->Hit(damage);

				percentage = rand() % 101;
				if (percentage < stunPercent)
				{
					((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::STUN, 1);
					((PlayerScript*)vectorTargetScript[0])->AddStress(11);
				}
			}

			Ptr<Sound> eye = ResourceManager::GetInst()->Load<Sound>(L"Sound_Eye", L"sound\\madman\\char_en_prophet_eye.wav");
			channel = eye->Play(1);
			eye->SetVolume(0.5f, channel);
		}
		else
		{
			// 적이 공격 회피
			((PlayerScript*)vectorTargetScript[0])->Dodge(true);

			Ptr<Sound> eye_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Eye_Miss", L"sound\\madman\\char_en_prophet_eye_miss.wav");
			channel = eye_miss->Play(1);
			eye_miss->SetVolume(0.5f, channel);
		}

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE_MONSTER::FULMINATE:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || _target == nullptr)
			return;

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int targetDodge = ((PlayerScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 103;
		UINT totalAcc = baseAcc - targetDodge;

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		UINT blightPercent = 100 - ((PlayerScript*)vectorTargetScript[0])->GetBlightResistance();
		int damage = int(1.f * (1.f - ((PlayerScript*)vectorTargetScript[0])->GetProtection() / 100.f));
		UINT critical = 6;

		GetAnimator2D()->Play(L"Attack_Shout");

		if (0 == targetPosition)
		{
			WideAttack(_target, totalAcc, critical, blightPercent, damage);
			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetNextObj();
			
			if (nullptr != first)
			{
				WideAttack(first, totalAcc, critical, blightPercent, damage);
				const vector<Script*>& vectorFirstTargetScript = first->GetScripts();
				GameObject* second = ((PlayerScript*)vectorFirstTargetScript[0])->GetNextObj();
				if (nullptr != second)
				{
					WideAttack(second, totalAcc, critical, blightPercent, damage);
					const vector<Script*>& vectorSecondTargetScript = second->GetScripts();
					GameObject* third = ((PlayerScript*)vectorSecondTargetScript[0])->GetNextObj();
					if (nullptr != third)
					{
						WideAttack(third, totalAcc, critical, blightPercent, damage);
					}
				}
			}
		}
		else if (1 == targetPosition)
		{
			WideAttack(_target, totalAcc, critical, blightPercent, damage);
			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != first)
			{
				WideAttack(first, totalAcc, critical, blightPercent, damage);
			}

			GameObject* second = ((PlayerScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != second)
			{
				WideAttack(second, totalAcc, critical, blightPercent, damage);
				const vector<Script*>& vectorSecondTargetScript = second->GetScripts();
				GameObject* third = ((PlayerScript*)vectorSecondTargetScript[0])->GetNextObj();
				if (nullptr != third)
				{
					WideAttack(third, totalAcc, critical, blightPercent, damage);
				}
			}
		}
		else if (2 == targetPosition)
		{
			WideAttack(_target, totalAcc, critical, blightPercent, damage);
			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != first)
			{
				WideAttack(first, totalAcc, critical, blightPercent, damage);
				const vector<Script*>& vectorFirstTargetScript = first->GetScripts();
				GameObject* second = ((PlayerScript*)vectorFirstTargetScript[0])->GetPrevObj();
				if (nullptr != second)
				{
					WideAttack(second, totalAcc, critical, blightPercent, damage);
					const vector<Script*>& vectorSecondTargetScript = second->GetScripts();

				}
			}

			GameObject* third = ((PlayerScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != third)
			{
				WideAttack(third, totalAcc, critical, blightPercent, damage);
			}
		}
		else if (3 == targetPosition)
		{
			WideAttack(_target, totalAcc, critical, blightPercent, damage);
			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetPrevObj();

			if (nullptr != first)
			{
				WideAttack(first, totalAcc, critical, blightPercent, damage);
				const vector<Script*>& vectorFirstTargetScript = first->GetScripts();
				GameObject* second = ((PlayerScript*)vectorFirstTargetScript[0])->GetPrevObj();
				if (nullptr != second)
				{
					WideAttack(second, totalAcc, critical, blightPercent, damage);
					const vector<Script*>& vectorSecondTargetScript = second->GetScripts();
					GameObject* third = ((PlayerScript*)vectorSecondTargetScript[0])->GetPrevObj();
					if (nullptr != third)
					{
						WideAttack(third, totalAcc, critical, blightPercent, damage);
					}
				}
			}
		}
		SetAnimStart(true);
	}
		break;
	}

	SetMove(true);
}

void MadmanScript::SelectTarget(const vector<GameObject*>& _playerParty)
{
	srand((UINT)time(NULL));
	SKILL_TYPE_MONSTER type = SKILL_TYPE_MONSTER::END;

	UINT targetSelect = rand() % 101;
	UINT skillSelect = rand() % 101;

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

	if (skillSelect < 34)
	{
		type = SKILL_TYPE_MONSTER::RUBBLE_OF_RUIN;
	}
	else if(skillSelect >= 34 && skillSelect < 67)
	{
		type = SKILL_TYPE_MONSTER::EYE_ON_YOU;
	}
	else if (skillSelect >= 67)
	{
		type = SKILL_TYPE_MONSTER::FULMINATE;
	}

	SetSkillType(type);
}

void MadmanScript::WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, UINT _blight, int _damage)
{
	srand((UINT)time(NULL));
	int channel = 0;
	UINT percentage = rand() % 101;

	const vector<Script*>& vectorTargetScript = _target->GetScripts();

	if (percentage < _accuracy)
	{
		if (percentage < _critical)
		{
			_damage = int(1 * 1.5f);

			((PlayerScript*)vectorTargetScript[0])->Hit(_damage);
			percentage = rand() % 101;
			if (percentage < _blight)
			{
				((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::BLIGHT, 2, 1);
			}
		}
		else
		{
			((PlayerScript*)vectorTargetScript[0])->Hit(_damage);

			percentage = rand() % 101;
			if (percentage < _blight)
			{
				((PlayerScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_PLAYER::BLIGHT, 2, 1);
			}
		}
	}
	else
	{
		// 적이 공격 회피
		((PlayerScript*)vectorTargetScript[0])->Dodge(true);
	}

	Ptr<Sound> fulminate = ResourceManager::GetInst()->Load<Sound>(L"Sound_Fulminate", L"sound\\madman\\char_en_prophet_fulminate.wav");
	channel = fulminate->Play(1);
	fulminate->SetVolume(0.5f, channel);
}
