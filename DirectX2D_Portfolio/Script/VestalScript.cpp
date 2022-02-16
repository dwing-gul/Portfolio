#include "pch.h"
#include "VestalScript.h"
#include "MonsterScript.h"
#include "LightScript.h"
#include <Engine/Animator2D.h>
#include <Engine/SceneManager.h>

VestalScript::VestalScript() :
	PlayerScript(SCRIPT_TYPE::VESTALSCRIPT)
{
	SetMaxHP(24);
	SetDodge(0);
	SetProtection(0);
	SetSpeed(4);
	SetAccuracyModifier(0);
	SetCriticalChance(1);
	SetDamage(POINT{ 4, 9 });
	SetStunResistance(40);
	SetDiseaseResistance(30);
	SetMoveResistance(40);
	SetBleedResistance(40);
	SetBlightResistance(40);
	SetDebuffResistance(40);
	SetTrapRelease(20);
}

VestalScript::~VestalScript()
{
}

void VestalScript::start()
{
	PlayerScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\vestal\\Attack_Heal.anim");
	GetAnimator2D()->LoadAnim(L"anim\\vestal\\Attack_Lectionary.anim");
	GetAnimator2D()->LoadAnim(L"anim\\vestal\\Attack_Mace.anim");
	GetAnimator2D()->LoadAnim(L"anim\\vestal\\Battle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\vestal\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\vestal\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\vestal\\Walk.anim");
	GetAnimator2D()->Play(L"Idle");

	SetSkillSet(SKILL_TYPE::MACE_BASH);
	SetSkillSet(SKILL_TYPE::DAZZLING_LIGHT);
	SetSkillSet(SKILL_TYPE::DIVINE_GRACE);
	SetSkillSet(SKILL_TYPE::DIVINE_COMFORT);
}

void VestalScript::update()
{
	PlayerScript::update();
}

void VestalScript::lateUpdate()
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

void VestalScript::SkillTrigger(GameObject* _target)
{
	SKILL_TYPE type = GetSkillType();

	int channel = 0;

	switch (type)
	{
	case SKILL_TYPE::MACE_BASH :
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
		int damage = int(float(rand() % 6 + GetDamage().x) * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
		UINT critical = GetCriticalChance();

		if (((MonsterScript*)vectorTargetScript[0])->GetMonsterType() == MONSTER_TYPE::PROFANE)
			damage = int(damage * 1.15f);

		if (2 == position || 3 == position)
			return;

		if (0 == enemyPosition || 1 == enemyPosition)
		{
			percentage = rand() % 101;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					damage = int(GetDamage().y * 1.5f);

					if (((MonsterScript*)vectorTargetScript[0])->GetMonsterType() == MONSTER_TYPE::PROFANE)
						damage = int(damage * 1.15f);

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

		Ptr<Sound> mace_bash = ResourceManager::GetInst()->Load<Sound>(L"Sound_Mace_Bash", L"sound\\vestal\\char_al_vst_macebash.wav");
		channel = mace_bash->Play(1);
		mace_bash->SetVolume(0.5f, channel);

		GetAnimator2D()->Play(L"Attack_Mace");

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE::DAZZLING_LIGHT :
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

		if (0 == position)
			return;

		if (3 != enemyPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 6 + GetDamage().x) * 0.25f *(1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = GetCriticalChance() + 5;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(GetDamage().y * 1.5f);
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

					if (percentage <= stunPercent)
						((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::STUN, 1);
				}
			}
			else
			{
				// 적이 공격 회피
				((MonsterScript*)vectorTargetScript[0])->Dodge(true);
			}

			// 횃불 밝기 밝히기
			GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
			const vector<Script*>& vectorLightScript = light->GetScripts();
			((LightScript*)vectorLightScript[0])->AddLight(6.f);

			SetMove(true);
		}
		else
		{
			return;
		}

		Ptr<Sound> dazzling_light = ResourceManager::GetInst()->Load<Sound>(L"Sound_Dazzling_Light", L"sound\\vestal\\char_al_vst_dazzlinglight.wav");
		channel = dazzling_light->Play(1);
		dazzling_light->SetVolume(0.5f, channel);

		GetAnimator2D()->Play(L"Attack_Lectionary");

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE::DIVINE_GRACE:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		int heal = rand() % 2 + 4;
		UINT critical = GetCriticalChance() + 12;
		UINT percentage = 0;

		if (0 == position || 1 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Heal");

		if (percentage < critical)
		{
			// 크리티컬 데미지
			heal = 10;
			((PlayerScript*)vectorTargetScript[0])->Heal(heal);
			((PlayerScript*)vectorTargetScript[0])->SubStress(4);
		}
		else
		{
			// 적에게 공격 적중
			((PlayerScript*)vectorTargetScript[0])->Heal(heal);
		}

		Ptr<Sound> divine_grace = ResourceManager::GetInst()->Load<Sound>(L"Sound_Divine_Grace", L"sound\\vestal\\char_al_vst_divinegrace.wav");
		channel = divine_grace->Play(1);
		divine_grace->SetVolume(0.5f, channel);

		SetAnimStart(true);
		SetMove(true);

		const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
		for (size_t i = 0; i < vectorPlayer.size(); ++i)
		{
			((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->SetHealTarget(false);
		}
	}
	break;
	case SKILL_TYPE::DIVINE_COMFORT:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		int heal = rand() % 3 + 1;
		UINT critical = GetCriticalChance() + 5;

		if (0 == position)
			return;

		GetAnimator2D()->Play(L"Attack_Heal");

		if (0 == targetPosition)
		{
			WideHeal(_target, critical, heal);

			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetNextObj();
			
			if (nullptr != first)
			{
				const vector<Script*>& vectorFirstTargetScript = first->GetScripts();
				GameObject* second = ((PlayerScript*)vectorFirstTargetScript[0])->GetNextObj();
				if (nullptr != second)
				{
					const vector<Script*>& vectorSecondTargetScript = second->GetScripts();
					GameObject* third = ((PlayerScript*)vectorSecondTargetScript[0])->GetNextObj();
					if (nullptr != third)
					{
						WideHeal(third, critical, heal);
					}
					
					WideHeal(second, critical, heal);
				}
				WideHeal(first, critical, heal);
			}

			SetMove(true);
		}
		else if (1 == targetPosition)
		{
			WideHeal(_target, critical, heal);
			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != first)
			{
				const vector<Script*>& vectorFirstTargetScript = first->GetScripts();
				WideHeal(first, critical, heal);
			}

			GameObject* second = ((PlayerScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != second)
			{
				const vector<Script*>& vectorSecondTargetScript = second->GetScripts();
				GameObject* third = ((PlayerScript*)vectorSecondTargetScript[0])->GetNextObj();
				if (nullptr != third)
				{
					WideHeal(third, critical, heal);
				}

				WideHeal(second, critical, heal);
			}

			SetMove(true);
		}
		else if (2 == targetPosition)
		{
			WideHeal(_target, critical, heal);
			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != first)
			{
				const vector<Script*>& vectorFirstTargetScript = first->GetScripts();
				GameObject* second = ((PlayerScript*)vectorFirstTargetScript[0])->GetPrevObj();
				if (nullptr != second)
				{
					const vector<Script*>& vectorSecondTargetScript = second->GetScripts();

					WideHeal(second, critical, heal);
				}
				WideHeal(first, critical, heal);
			}

			GameObject* third = ((PlayerScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != third)
			{
				WideHeal(third, critical, heal);
			}

			SetMove(true);
		}
		else if (3 == targetPosition)
		{
			WideHeal(_target, critical, heal);
			GameObject* first = ((PlayerScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != first)
			{
				const vector<Script*>& vectorFirstTargetScript = first->GetScripts();
				GameObject* second = ((PlayerScript*)vectorFirstTargetScript[0])->GetPrevObj();
				if (nullptr != second)
				{
					const vector<Script*>& vectorSecondTargetScript = second->GetScripts();
					GameObject* third = ((PlayerScript*)vectorSecondTargetScript[0])->GetPrevObj();
					if (nullptr != third)
					{
						WideHeal(third, critical, heal);
					}

					WideHeal(second, critical, heal);
				}
				WideHeal(first, critical, heal);
			}

			SetMove(true);
		}

		Ptr<Sound> divine_comfort = ResourceManager::GetInst()->Load<Sound>(L"Sound_Divine_Comfort", L"sound\\vestal\\char_al_vst_godscom.wav");
		channel = divine_comfort->Play(1);
		divine_comfort->SetVolume(0.5f, channel);

		SetAnimStart(true);

		const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
		for (size_t i = 0; i < vectorPlayer.size(); ++i)
		{
			((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->SetHealTarget(false);
		}
	}
	break;
	}

	const vector<GameObject*>& vectorMonster = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);
	for (size_t i = 0; i < vectorMonster.size(); ++i)
	{
		((MonsterScript*)vectorMonster[i]->GetScripts()[0])->SetBoolTarget(false);
	}
}

void VestalScript::WideHeal(GameObject* _target, UINT _critical, int _heal, int _stress)
{
	srand((UINT)time(NULL));
	UINT percentage = rand() % 101;
	const vector<Script*>& vectorTargetScript = _target->GetScripts();

	if (percentage < _critical)
	{
		_heal = 6;
		((PlayerScript*)vectorTargetScript[0])->Heal(_heal);

		((PlayerScript*)vectorTargetScript[0])->SubStress(_stress);
	}
	else
	{
		((PlayerScript*)vectorTargetScript[0])->Heal(_heal);

		((PlayerScript*)vectorTargetScript[0])->SubStress(_stress);
	}
}
