#include "pch.h"
#include "CrusaderScript.h"
#include "MonsterScript.h"
#include <Engine/Animator2D.h>
#include <Engine/Transform.h>
#include <Engine/SceneManager.h>

CrusaderScript::CrusaderScript() :
	PlayerScript(SCRIPT_TYPE::CRUSADERSCRIPT)
{
	SetMaxHP(33);
	SetDodge(5);
	SetProtection(0);
	SetSpeed(1);
	SetAccuracyModifier(0);
	SetCriticalChance(3);
	SetDamage(POINT{ 6, 12 });
	SetStunResistance(50);
	SetDiseaseResistance(30);
	SetMoveResistance(50);
	SetBleedResistance(40);
	SetBlightResistance(40);
	SetDebuffResistance(40);
	SetTrapRelease(20);
}

CrusaderScript::~CrusaderScript()
{
}

void CrusaderScript::start()
{
	PlayerScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Walk.anim");
	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Attack.anim");
	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Attack_Charge.anim");
	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Attack_Scroll.anim");
	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Attack_Stun.anim");
	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Battle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\crusader\\Defend.anim");

	GetAnimator2D()->Play(L"Idle");

	SetSkillSet(SKILL_TYPE::SMITE);
	SetSkillSet(SKILL_TYPE::ZEALOUS_ACCUSATION);
	SetSkillSet(SKILL_TYPE::HOLY_LANCE);
	SetSkillSet(SKILL_TYPE::STUNNING_BLOW);
}

void CrusaderScript::update()
{
	PlayerScript::update();
}

void CrusaderScript::lateUpdate()
{
	PlayerScript::lateUpdate();

	if (IsBattle())
	{
		if (!IsTurnStart())
			return;

		if (nullptr != GetTarget())
			SkillTrigger(GetTarget());

		SetTarget(nullptr);
	}
}

void CrusaderScript::SkillTrigger(GameObject* _target)
{
	SKILL_TYPE type = GetSkillType();
	int channel = 0;

	switch (type)
	{
	case SKILL_TYPE::SMITE:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::MONSTER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int enemyDodge = ((MonsterScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 85;
		int totalAcc = baseAcc + GetAccuracyModifier() - enemyDodge + 5;

		int position = GetPosition();
		int enemyPosition = ((MonsterScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;

		if (2 == position || 3 == position)
			return;

		if (0 == enemyPosition || 1 == enemyPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 7 + GetDamage().x) * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = GetCriticalChance();

			if (((MonsterScript*)vectorTargetScript[0])->GetMonsterType() == MONSTER_TYPE::PROFANE)
				damage = int(damage * 1.15f);

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(GetDamage().y * 1.5f);

					if (((MonsterScript*)vectorTargetScript[0])->GetMonsterType() == MONSTER_TYPE::PROFANE)
						damage = int(damage * 1.15f);

					((MonsterScript*)vectorTargetScript[0])->SetCritical(true);
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
					SubStress(4);
				}
				else
				{
					// 적에게 공격 적중
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
				}

				Ptr<Sound> smite = ResourceManager::GetInst()->Load<Sound>(L"Sound_Smite", L"sound\\crusader\\char_al_cru_smite.wav");
				channel = smite->Play(1);
				smite->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((MonsterScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> smite_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Smite_Miss", L"sound\\crusader\\char_al_cru_smite_miss.wav");
				channel = smite_miss->Play(1);
				smite_miss->SetVolume(0.5f, channel);
			}
			SetMove(true);
		}
		else
		{
			return;
		}

		GetAnimator2D()->Play(L"Attack");

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE::ZEALOUS_ACCUSATION:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::MONSTER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int enemyDodge = ((MonsterScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 85;
		int totalAcc = baseAcc + GetAccuracyModifier() - enemyDodge + 5;

		int position = GetPosition();
		int enemyPosition = ((MonsterScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		int damage = int(float(rand() % 7 + GetDamage().x) * 0.6f * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
		UINT critical = GetCriticalChance() - 4;

		if (2 == position || 3 == position)
			return;

		if (0 == enemyPosition)
		{
			WideAttack(_target, totalAcc, critical, damage);

			GameObject* another = ((MonsterScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != another)
			{
				WideAttack(another, totalAcc, critical, damage);
			}
			SetMove(true);
		}
		else if (1 == enemyPosition)
		{
			WideAttack(_target, totalAcc, critical, damage);
			GameObject* another = ((MonsterScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != another)
			{
				WideAttack(another, totalAcc, critical, damage);
			}
			SetMove(true);
		}
		else
		{
			return;
		}

		Ptr<Sound> zealous_accusation = ResourceManager::GetInst()->Load<Sound>(L"Sound_Zealous_Accusation", L"sound\\crusader\\char_al_cru_zealousacc.wav");
		channel = zealous_accusation->Play(1, 0.5f, true);

		GetAnimator2D()->Play(L"Attack_Scroll");

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE::HOLY_LANCE:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::MONSTER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int enemyDodge = ((MonsterScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 85;
		int totalAcc = baseAcc + GetAccuracyModifier() - enemyDodge + 5;

		int position = GetPosition();
		int enemyPosition = ((MonsterScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;

		if (0 == position || 1 == position)
			return;

		if (0 != enemyPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 7 + GetDamage().x) * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = GetCriticalChance() + 7;

			if (((MonsterScript*)vectorTargetScript[0])->GetMonsterType() == MONSTER_TYPE::PROFANE)
				damage = int(damage * 1.15f);

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(GetDamage().y * 1.5f);

					if (((MonsterScript*)vectorTargetScript[0])->GetMonsterType() == MONSTER_TYPE::PROFANE)
						damage = int(damage * 1.15f);

					((MonsterScript*)vectorTargetScript[0])->SetCritical(true);
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
					SubStress(4);
				}
				else
				{
					// 적에게 공격 적중
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
				}
			}
			else
			{
				// 적이 공격 회피
				((MonsterScript*)vectorTargetScript[0])->Dodge(true);
			}
			SetMove(true);
		}
		else
		{
			return;
		}

		Ptr<Sound> holy_lance = ResourceManager::GetInst()->Load<Sound>(L"Sound_Holy_Lance", L"sound\\crusader\\char_al_cry_holylance.wav");
		channel = holy_lance->Play(1, 0.5f);

		GetAnimator2D()->Play(L"Attack_Charge");

		SetAnimStart(true);
	}
		break;
	case SKILL_TYPE::STUNNING_BLOW:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::MONSTER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int enemyDodge = ((MonsterScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 90;
		int totalAcc = baseAcc + GetAccuracyModifier() - enemyDodge + 5;

		int position = GetPosition();
		int enemyPosition = ((MonsterScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		UINT stunPercent = 100 - ((MonsterScript*)vectorTargetScript[0])->GetStunResistance();

		if (0 == position || 1 == position)
			return;

		if (0 == enemyPosition || 1 == enemyPosition)
		{
			percentage = rand() % 101;
			int damage = int(float((rand() % 7 + GetDamage().x) / 2) * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = GetCriticalChance();

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지 및 스턴 적용
					damage = int(GetDamage().y * 1.5f);
					((MonsterScript*)vectorTargetScript[0])->SetCritical(true);
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
					SubStress(4);

					percentage = rand() % 101;

					if (percentage < stunPercent)
						((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::STUN, 1);
				}
				else
				{
					// 적에게 공격 적중
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);

					percentage = rand() % 101;

					if (percentage < stunPercent)
						((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::STUN, 1);
				}
			}
			else
			{
				// 적이 공격 회피
				((MonsterScript*)vectorTargetScript[0])->Dodge(true);
			}
			SetMove(true);
		}
		else
		{
			return;
		}

		Ptr<Sound> stunning_blow = ResourceManager::GetInst()->Load<Sound>(L"Sound_Stunning_Blow", L"sound\\crusader\\char_al_cru_stunningblow.wav");
		channel = stunning_blow->Play(1, 0.5f);

		GetAnimator2D()->Play(L"Attack_Stun");

		SetAnimStart(true);
	}
		break;
	}

	const vector<GameObject*>& vectorMonster = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);
	for (size_t i = 0; i < vectorMonster.size(); ++i)
	{
		((MonsterScript*)vectorMonster[i]->GetScripts()[0])->SetBoolTarget(false);
	}
}

void CrusaderScript::WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, int _damage)
{
	srand((UINT)time(NULL));
	const vector<Script*>& vectorTargetScript = _target->GetScripts();
	UINT percentage = rand() % 101;

	if (percentage < _accuracy)
	{
		if (percentage < _critical)
		{
			// 크리티컬 데미지
			_damage = int(GetDamage().y * 1.5f);
			((MonsterScript*)vectorTargetScript[0])->SetCritical(true);
			((MonsterScript*)vectorTargetScript[0])->Hit(_damage);
			SubStress(4);
		}
		else
		{
			// 적에게 공격 적중
			((MonsterScript*)vectorTargetScript[0])->Hit(_damage);
		}
	}
	else
	{
		// 적이 공격 회피
		((MonsterScript*)vectorTargetScript[0])->Dodge(true);
	}
}
