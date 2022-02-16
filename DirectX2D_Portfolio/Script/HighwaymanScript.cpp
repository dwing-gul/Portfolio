#include "pch.h"
#include "HighwaymanScript.h"
#include "MonsterScript.h"
#include <Engine/Animator2D.h>
#include <Engine/Transform.h>
#include <Engine/SceneManager.h>

HighwaymanScript::HighwaymanScript() :
	PlayerScript(SCRIPT_TYPE::HIGHWAYMANSCRIPT)
{
	SetMaxHP(23);
	SetDodge(10);
	SetProtection(0);
	SetSpeed(5);
	SetAccuracyModifier(0);
	SetCriticalChance(5);
	SetDamage(POINT{ 5, 10 });
	SetStunResistance(40);
	SetDiseaseResistance(30);
	SetMoveResistance(40);
	SetBleedResistance(40);
	SetBlightResistance(40);
	SetDebuffResistance(40);
	SetTrapRelease(60);
}

HighwaymanScript::~HighwaymanScript()
{
}

void HighwaymanScript::start()
{
	PlayerScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Attack_Lunge.anim");
	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Attack_Lunge2.anim");
	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Attack_Pistol.anim");
	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Attack_Slice.anim");
	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Battle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\highwayman\\Walk.anim");
	GetAnimator2D()->Play(L"Idle");

	SetSkillSet(SKILL_TYPE::WICKED_SLICE);
	SetSkillSet(SKILL_TYPE::PISTOL_SHOT);
	SetSkillSet(SKILL_TYPE::GRAPESHOT_BLAST);
	SetSkillSet(SKILL_TYPE::OPEN_VEIN);
}

void HighwaymanScript::update()
{
	PlayerScript::update();
}

void HighwaymanScript::lateUpdate()
{
	PlayerScript::lateUpdate();

	if (IsBattle())
	{
		if (!IsTurnStart())
		{
			GameObject* shotEffect = SceneManager::GetInst()->FindObjByName(L"HighwaymanShotEffect");
			if (nullptr != shotEffect && !(shotEffect->IsDead()))
			{
				DeleteObject(shotEffect);
			}

			return;
		}

		if (nullptr != GetTarget())
			SkillTrigger(GetTarget());

		if (!IsAnimStart())
		{
			GameObject* shotEffect = SceneManager::GetInst()->FindObjByName(L"HighwaymanShotEffect");
			if (nullptr != shotEffect && !(shotEffect->IsDead()))
			{
				DeleteObject(shotEffect);
			}
		}

		SetTarget(nullptr);
	}
	else
	{
		GameObject* shotEffect = SceneManager::GetInst()->FindObjByName(L"HighwaymanShotEffect");
		if (nullptr != shotEffect && !(shotEffect->IsDead()))
		{
			DeleteObject(shotEffect);
		}
	}
}

void HighwaymanScript::SkillTrigger(GameObject* _target)
{
	SKILL_TYPE type = GetSkillType();

	int channel = 0;

	switch (type)
	{
	case SKILL_TYPE::WICKED_SLICE :
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

		if (3 == position)
			return;

		if (0 == enemyPosition || 1 == enemyPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 7 + GetDamage().x) * 1.15f * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = GetCriticalChance() + 5;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(GetDamage().y * 1.5f);
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

		Ptr<Sound> wicked_slice = ResourceManager::GetInst()->Load<Sound>(L"Sound_Wicked_Slice", L"sound\\highwayman\\char_al_hwy_wickedslice.wav");
		channel = wicked_slice->Play(1);
		wicked_slice->SetVolume(0.5f, channel);

		GetAnimator2D()->Play(L"Attack_Lunge");

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE::PISTOL_SHOT :
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

		if (0 == position)
			return;

		if (0 != enemyPosition)
		{
			GameObject* another = ((MonsterScript*)vectorTargetScript[0])->GetNextObj();
			percentage = rand() % 101;
			int damage = int(float(rand() % 7 + GetDamage().x) * 0.6f * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = GetCriticalChance() - 4;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					// 크리티컬 데미지
					damage = int(GetDamage().y * 1.5f);
					((MonsterScript*)vectorTargetScript[0])->SetCritical(true);
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
					SubStress(4);
				}
				else
				{
					// 적에게 공격 적중
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
				}

				Ptr<Sound> pistol_shot = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pistol_Shot", L"sound\\highwayman\\char_al_hwy_pistolshot.wav");
				channel = pistol_shot->Play(1);
				pistol_shot->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((MonsterScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> pistol_shot_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pistol_Shot_Miss", L"sound\\highwayman\\char_al_hwy_pistolshot_miss.wav");
				channel = pistol_shot_miss->Play(1);
				pistol_shot_miss->SetVolume(0.5f, channel);
			}

			SetMove(true);
		}
		else
		{
			return;
		}

		Ptr<Prefab> pistol_shot = ResourceManager::GetInst()->Load<Prefab>(L"HighwaymanShotEffect", L"prefab\\HighwaymanShotEffect.prefab");
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 pos = camera->GetTransform()->GetLocalPos();
		pos.x += 110.f;
		pos.y += 40.f;
		Instantiate(pistol_shot, pos, 2);
		
		GetAnimator2D()->Play(L"Attack_Pistol");

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE::GRAPESHOT_BLAST :
	{
		if (_target->GetObjectType() != OBJECT_TYPE::MONSTER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int enemyDodge = ((MonsterScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 75;
		int totalAcc = baseAcc + GetAccuracyModifier() - enemyDodge + 5;

		int position = GetPosition();
		int enemyPosition = ((MonsterScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		int damage = int(float(rand() % 7 + GetDamage().x) * 0.6f * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
		UINT critical = GetCriticalChance() - 9;

		if (0 == position || 3 == position)
			return;

		if (0 == enemyPosition)
		{
			WideAttack(_target, totalAcc, critical, damage);

			GameObject* chain = ((MonsterScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != chain)
			{
				WideAttack(chain, totalAcc, critical, damage);
				const vector<Script*>& vectorChainTargetScript = chain->GetScripts();
				GameObject* chain2 = ((MonsterScript*)vectorChainTargetScript[0])->GetNextObj();
				if(nullptr != chain2)
					WideAttack(chain2, totalAcc, critical, damage);
			}

			SetMove(true);
		}
		else if (1 == enemyPosition)
		{
			WideAttack(_target, totalAcc, critical, damage);

			GameObject* chainPrev = ((MonsterScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != chainPrev)
			{
				WideAttack(chainPrev, totalAcc, critical, damage);
			}
			GameObject* chainNext = ((MonsterScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != chainNext)
				WideAttack(chainNext, totalAcc, critical, damage);

			SetMove(true);
		}
		else if (2 == enemyPosition)
		{
			WideAttack(_target, totalAcc, critical, damage);

			GameObject* chain = ((MonsterScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != chain)
			{
				WideAttack(chain, totalAcc, critical, damage);
				const vector<Script*>& vectorChainTargetScript = chain->GetScripts();
				GameObject* chain2 = ((MonsterScript*)vectorChainTargetScript[0])->GetPrevObj();
				if (nullptr != chain2)
					WideAttack(chain2, totalAcc, critical, damage);
			}
			SetMove(true);
		}
		else
		{
			return;
		}


		Ptr<Prefab> pistol_shot = ResourceManager::GetInst()->Load<Prefab>(L"HighwaymanShotEffect", L"prefab\\HighwaymanShotEffect.prefab");
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 pos = camera->GetTransform()->GetLocalPos();
		pos.x += 110.f;
		pos.y += 40.f;
		Instantiate(pistol_shot, pos, 2);

		GetAnimator2D()->Play(L"Attack_Pistol");

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE::OPEN_VEIN:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::MONSTER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int enemyDodge = ((MonsterScript*)vectorTargetScript[0])->GetDodge();
		int baseAcc = 95;
		int totalAcc = baseAcc + GetAccuracyModifier() - enemyDodge + 5;

		int position = GetPosition();
		int enemyPosition = ((MonsterScript*)vectorTargetScript[0])->GetPosition();
		UINT percentage = 0;
		UINT bleedPercent = 100 - ((MonsterScript*)vectorTargetScript[0])->GetBleedResistance();
		UINT debuffPercent = 100 - ((MonsterScript*)vectorTargetScript[0])->GetDebuffResistance();

		if (3 == position)
			return;

		if (1 == enemyPosition || 0 == enemyPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 7 + GetDamage().x) * 0.85f * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
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
					if (bleedPercent > percentage)
						((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::BLEED, 3, 2);

					percentage = rand() % 101;
					if (debuffPercent > percentage)
					{
						int speed = ((MonsterScript*)vectorTargetScript[0])->GetSpeed();
						((MonsterScript*)vectorTargetScript[0])->SetSpeed(speed - 1);

						int bleedResis = ((MonsterScript*)vectorTargetScript[0])->GetBleedResistance();
						((MonsterScript*)vectorTargetScript[0])->SetBleedResistance(bleedResis - 20);
					}
				}
				else
				{
					// 적에게 공격 적중
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);

					percentage = rand() % 101;
					if (bleedPercent > percentage)
						((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::BLEED, 3, 2);

					percentage = rand() % 101;
					if (debuffPercent > percentage)
					{
						int speed = ((MonsterScript*)vectorTargetScript[0])->GetSpeed();
						((MonsterScript*)vectorTargetScript[0])->SetSpeed(speed - 1);

						int bleedResis = ((MonsterScript*)vectorTargetScript[0])->GetBleedResistance();
						((MonsterScript*)vectorTargetScript[0])->SetBleedResistance(bleedResis - 20);
					}
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

		Ptr<Sound> open_vein = ResourceManager::GetInst()->Load<Sound>(L"Sound_Open_Vein", L"sound\\highwayman\\char_al_hwy_openedvein.wav");
		channel = open_vein->Play(1);
		open_vein->SetVolume(0.5f, channel);

		GetAnimator2D()->Play(L"Attack_Slice");

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

void HighwaymanScript::WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, int _damage)
{
	srand((UINT)time(NULL));

	int channel = 0;

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

		Ptr<Sound> grape_shot = ResourceManager::GetInst()->Load<Sound>(L"Sound_Grape_Shot", L"sound\\highwayman\\char_al_hwy_grapeshot.wav");
		channel = grape_shot->Play(1);
		grape_shot->SetVolume(0.5f, channel);
	}
	else
	{
		// 적이 공격 회피
		((MonsterScript*)vectorTargetScript[0])->Dodge(true);

		Ptr<Sound> grape_shot_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Grape_Shot_Miss", L"sound\\highwayman\\char_al_hwy_grapeshot_miss.wav");
		channel = grape_shot_miss->Play(1);
		grape_shot_miss->SetVolume(0.5f, channel);
	}
}
