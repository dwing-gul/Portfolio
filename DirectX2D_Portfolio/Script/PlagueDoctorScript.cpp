#include "pch.h"
#include "PlagueDoctorScript.h"
#include "MonsterScript.h"
#include <Engine/Animator2D.h>
#include <Engine/Transform.h>
#include <Engine/SceneManager.h>

PlagueDoctorScript::PlagueDoctorScript() :
	PlayerScript(SCRIPT_TYPE::PLAGUEDOCTORSCRIPT)
{
	SetMaxHP(22);
	SetDodge(5);
	SetProtection(0);
	SetSpeed(7);
	SetAccuracyModifier(0);
	SetCriticalChance(2);
	SetDamage(POINT{ 4, 7 });
	SetStunResistance(30);
	SetDiseaseResistance(50);
	SetMoveResistance(30);
	SetBleedResistance(30);
	SetBlightResistance(70);
	SetDebuffResistance(60);
	SetTrapRelease(30);
}

PlagueDoctorScript::~PlagueDoctorScript()
{
}

void PlagueDoctorScript::start()
{
	PlayerScript::start();

	GetAnimator2D()->LoadAnim(L"anim\\plague_doctor\\Attack_Buff.anim");
	GetAnimator2D()->LoadAnim(L"anim\\plague_doctor\\Attack_Dagger.anim");
	GetAnimator2D()->LoadAnim(L"anim\\plague_doctor\\Attack_Grenade.anim");
	GetAnimator2D()->LoadAnim(L"anim\\plague_doctor\\Defend.anim");
	GetAnimator2D()->LoadAnim(L"anim\\plague_doctor\\Battle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\plague_doctor\\Idle.anim");
	GetAnimator2D()->LoadAnim(L"anim\\plague_doctor\\Walk.anim");
	GetAnimator2D()->Play(L"Idle");

	SetSkillSet(SKILL_TYPE::PLAGUE_GRENADE);
	SetSkillSet(SKILL_TYPE::INCISION);
	SetSkillSet(SKILL_TYPE::BATTLEFIELD_MEDICINE);
	SetSkillSet(SKILL_TYPE::DISORIENTING_BLAST);
}

void PlagueDoctorScript::update()
{
	PlayerScript::update();
}

void PlagueDoctorScript::lateUpdate()
{
	PlayerScript::lateUpdate();

	if (IsBattle())
	{
		if (!IsTurnStart())
		{
			GameObject* grenadeEffect = SceneManager::GetInst()->FindObjByName(L"GrenadeEffect");
			if (nullptr != grenadeEffect && !(grenadeEffect->IsDead()))
			{
				DeleteObject(grenadeEffect);
			}

			return;
		}

		if (nullptr != GetTarget())
			SkillTrigger(GetTarget());

		if (!IsAnimStart())
		{
			GameObject* grenadeEffect = SceneManager::GetInst()->FindObjByName(L"GrenadeEffect");
			if (nullptr != grenadeEffect && !(grenadeEffect->IsDead()))
			{
				DeleteObject(grenadeEffect);
			}
		}

		SetTarget(nullptr);
	}
	else
	{
		GameObject* grenadeEffect = SceneManager::GetInst()->FindObjByName(L"GrenadeEffect");
		if (nullptr != grenadeEffect && !(grenadeEffect->IsDead()))
		{
			DeleteObject(grenadeEffect);
		}
	}
}

void PlagueDoctorScript::SkillTrigger(GameObject* _target)
{
	SKILL_TYPE type = GetSkillType();

	int channel = 0;

	switch (type)
	{
	case SKILL_TYPE::PLAGUE_GRENADE:
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
		int damage = int(float(rand() % 5 + GetDamage().x) * 0.1f * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
		UINT critical = GetCriticalChance();
		UINT blightPercent = 100 - ((MonsterScript*)vectorTargetScript[0])->GetBlightResistance();

		if (3 == position)
			return;

		if (2 == enemyPosition)
		{
			WideAttack(_target, totalAcc, critical, blightPercent, damage);
			GameObject* chain = ((MonsterScript*)vectorTargetScript[0])->GetNextObj();
			if (nullptr != chain)
			{
				WideAttack(chain, totalAcc, critical, blightPercent, damage);
			}
			
			SetMove(true);
		}
		else if (3 == enemyPosition)
		{
			WideAttack(_target, totalAcc, critical, blightPercent, damage);
			GameObject* chainPrev = ((MonsterScript*)vectorTargetScript[0])->GetPrevObj();
			if (nullptr != chainPrev)
			{
				WideAttack(chainPrev, totalAcc, critical, blightPercent, damage);
			}

			SetMove(true);
		}
		else
		{
			return;
		}

		Ptr<Prefab> grenade_explosion = ResourceManager::GetInst()->Load<Prefab>(L"GrenadeEffect", L"prefab\\GrenadeEffect.prefab");
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 pos = camera->GetTransform()->GetLocalPos();
		pos.x += 110.f;
		pos.y -= 40.f;
		Instantiate(grenade_explosion, pos, 2);

		GetAnimator2D()->Play(L"Attack_Grenade");

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE::INCISION:
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
		UINT bleedPercent = 100 - ((MonsterScript*)vectorTargetScript[0])->GetBleedResistance();

		if (3 == position)
			return;

		if (0 == enemyPosition || 1 == enemyPosition)
		{
			percentage = rand() % 101;
			int damage = int(float(rand() % 5 + GetDamage().x) * (1.f - ((MonsterScript*)vectorTargetScript[0])->GetProtection() / 100.f));
			UINT critical = GetCriticalChance() + 5;

			if ((int)percentage < totalAcc)
			{
				if (percentage < critical)
				{
					damage = int(GetDamage().y * 1.5f);
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);
					SubStress(4);

					percentage = rand() % 101;
					if (percentage < bleedPercent)
						((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::BLEED, 3, 2);
				}
				else
				{
					// 적에게 공격 적중
					((MonsterScript*)vectorTargetScript[0])->Hit(damage);

					percentage = rand() % 101;
					if (percentage < bleedPercent)
						((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::BLEED, 3, 2);
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


		Ptr<Sound> incision = ResourceManager::GetInst()->Load<Sound>(L"Sound_Incision", L"sound\\plague_doctor\\char_al_plg_incision.wav");
		channel = incision->Play(1);
		incision->SetVolume(0.5f, channel);

		GetAnimator2D()->Play(L"Attack_Dagger");

		SetAnimStart(true);
	}
	break;
	case SKILL_TYPE::BATTLEFIELD_MEDICINE:
	{
		if (_target->GetObjectType() != OBJECT_TYPE::PLAYER || nullptr == _target)
			return;

		srand((UINT)time(NULL));

		const vector<Script*>& vectorTargetScript = _target->GetScripts();

		int position = GetPosition();
		int targetPosition = ((PlayerScript*)vectorTargetScript[0])->GetPosition();
		int heal = 1;
		UINT critical = GetCriticalChance();
		UINT percentage = 0;

		if (0 == position || 3 == position)
			return;

		if (percentage < critical)
		{
			// 크리티컬 데미지
			heal = 2;
			((PlayerScript*)vectorTargetScript[0])->Heal(heal);
			((PlayerScript*)vectorTargetScript[0])->SubStress(4);
		}
		else
		{
			((PlayerScript*)vectorTargetScript[0])->Heal(heal);
		}

		((PlayerScript*)vectorTargetScript[0])->ReleaseDebuffByType((UINT)DEBUFF_TYPE_PLAYER::BLEED | (UINT)DEBUFF_TYPE_PLAYER::BLIGHT);

		ReleaseDebuffByType((UINT)DEBUFF_TYPE_PLAYER::BLEED | (UINT)DEBUFF_TYPE_PLAYER::BLIGHT);

		Ptr<Sound> battle_medicine = ResourceManager::GetInst()->Load<Sound>(L"Sound_Battle_Medicine", L"sound\\plague_doctor\\char_al_plg_battlemed.wav");
		channel = battle_medicine->Play(1);
		battle_medicine->SetVolume(0.5f, channel);

		GetAnimator2D()->Play(L"Attack_Buff");

		SetAnimStart(true);
		SetMove(true);

		const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
		for (size_t i = 0; i < vectorPlayer.size(); ++i)
		{
			((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->SetHealTarget(false);
		}
	}
	break;
	case SKILL_TYPE::DISORIENTING_BLAST:
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
		UINT stunPercent = 100 - ((MonsterScript*)vectorTargetScript[0])->GetStunResistance();

		if (0 == position)
			return;

		if (0 != enemyPosition)
		{
			percentage = rand() % 101;
			int damage = 0;

			if ((int)percentage < totalAcc)
			{
				// 적에게 공격 적중
				((MonsterScript*)vectorTargetScript[0])->Hit(damage);
				// 디버프 작용

				percentage = rand() % 101;

				if (percentage < stunPercent)
					((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::STUN, 1);

				Ptr<Sound> disorienting_blast = ResourceManager::GetInst()->Load<Sound>(L"Sound_Disorienting_Blast", L"sound\\plague_doctor\\char_al_plg_disorientingblast.wav");
				channel = disorienting_blast->Play(1);
				disorienting_blast->SetVolume(0.5f, channel);
			}
			else
			{
				// 적이 공격 회피
				((MonsterScript*)vectorTargetScript[0])->Dodge(true);

				Ptr<Sound> disorienting_blast_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Disorienting_Blast_Miss", L"sound\\plague_doctor\\char_al_plg_disorientingblast_miss.wav");
				channel = disorienting_blast_miss->Play(1);
				disorienting_blast_miss->SetVolume(0.5f, channel);
			}

			SetMove(true);
		}
		else
		{
			return;
		}

		GetAnimator2D()->Play(L"Attack_Buff");

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

void PlagueDoctorScript::WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, UINT _debuff, int _damage)
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
			((MonsterScript*)vectorTargetScript[0])->Hit(_damage);
			SubStress(4);

			percentage = rand() % 101;
			if (percentage < _debuff)
			{
				((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::BLIGHT, 3, 4);
			}
		}
		else
		{
			// 적에게 공격 적중
			((MonsterScript*)vectorTargetScript[0])->Hit(_damage);

			percentage = rand() % 101;
			if (percentage < _debuff)
			{
				((MonsterScript*)vectorTargetScript[0])->SetDebuff(DEBUFF_TYPE_MONSTER::BLIGHT, 3, 4);
			}
		}

		Ptr<Sound> plague_grenade = ResourceManager::GetInst()->Load<Sound>(L"Sound_Plague_Grenade", L"sound\\plague_doctor\\char_al_plg_plaguegrenade.wav");
		channel = plague_grenade->Play(1);
		plague_grenade->SetVolume(0.5f, channel);
	}
	else
	{
		// 적이 공격 회피
		((MonsterScript*)vectorTargetScript[0])->Dodge(true);

		Ptr<Sound> plague_grenade_miss = ResourceManager::GetInst()->Load<Sound>(L"Sound_Plague_Grenade_Miss", L"sound\\plague_doctor\\char_al_plg_plaguegrenade_miss.wav");
		channel = plague_grenade_miss->Play(1);
		plague_grenade_miss->SetVolume(0.5f, channel);
	}
}
