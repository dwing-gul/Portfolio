#include "pch.h"
#include "MonsterScript.h"
#include "PlayerScript.h"
#include "UIScript.h"
#include "TextUIScript.h"
#include <Engine/Core.h>
#include <Engine/Collider2D.h>
#include <Engine/GameObject.h>
#include <Engine/Animator2D.h>
#include <Engine/Transform.h>
#include <Engine/TimeManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>

#define MONSTER_STATUS_STUN			1
#define MONSTER_STATUS_DEBUFF		2
#define MONSTER_STATUS_BLEED		4
#define MONSTER_STATUS_MOVE			8
#define MONSTER_STATUS_BLIGHT		16

bool MonsterScript::g_bOverlap = false;
bool MonsterScript::g_bHitEffect = true;

MonsterScript::MonsterScript(SCRIPT_TYPE _type) :
	Script((UINT)_type),
	m_prevObj(nullptr),
	m_nextObj(nullptr),
	m_target(nullptr),
	m_damageText(nullptr),
	m_bSelect(false),
	m_position(0),
	m_maxHP(10),
	m_curHP(0),
	m_type(MONSTER_TYPE::HUMAN),
	m_skillType(SKILL_TYPE_MONSTER::END),
	m_dodge(1),
	m_protection(10),
	m_speed(1),
	m_turnDecision(1),
	m_stunResistance(20),
	m_debuffResistance(20),
	m_bleedResistance(10),
	m_moveResistance(20),
	m_blightResistance(20),
	m_bHit(false),
	m_bCritical(false),
	m_bDodge(false),
	m_bTurnStart(false),
	m_bTurnEnd(false),
	m_bAnimStart(false),
	m_bInfoOpen(false),
	m_bMove(false),
	m_bFirstMove(true),
	m_bEffect(true),
	m_bFirstDamageText(true),
	m_bDamageText(false),
	m_bDebuffDamage(false),
	m_bTarget(false),
	m_animTime(1.f),
	m_moveTime(1.f),
	m_debuffDamageTime(0.5f),
	m_height(0.f),
	m_debuff(0),
	m_stunTurnCount(0),
	m_debuffTurnCount(0),
	m_bleedTurnCount(0),
	m_blightTurnCount(0),
	m_bleedDamage(0),
	m_blightDamage(0)
{
}

MonsterScript::~MonsterScript()
{
	if(nullptr != m_damageText)
		((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(false);
}

void MonsterScript::start()
{
	m_curHP = m_maxHP;

	Ptr<Prefab> selectEffect = ResourceManager::GetInst()->Load<Prefab>(L"SelectEffect", L"prefab\\SelectEffect.prefab");
	GameObject* clone = selectEffect->GetPrefabObject()->Clone();
	AddChild(Object(), clone);
}

void MonsterScript::update()
{
	m_bHit = false;

	if (m_bDodge)
	{
		m_bAnimStart = true;
		m_bMove = true;
		GetAnimator2D()->Play(L"Defend");
		// 회피 텍스트 띄우기
		InputDamageTextUI(0);
	}

	if (m_bMove)
	{
		m_moveTime -= DELTATIME;

		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 cameraPos = camera->GetTransform()->GetLocalPos();
		int count = 0;

		vector<GameObject*> vectorMonster = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);
		for (size_t i = 0; i < vectorMonster.size(); ++i)
		{
			if (((MonsterScript*)vectorMonster[i]->GetScripts()[0])->IsMove())
			{
				++count;
			}
		}

		if (1 == count)
		{
			Vector3 pos = GetTransform()->GetLocalPos();

			if (m_bFirstMove)
			{
				m_bFirstMove = false;
				m_height = pos.y - 40.f;
			}
			GetTransform()->SetLocalPos(Vector3(cameraPos.x + 150.f, m_height, 80.f));
		}
		else if (count > 1)
		{
			int battlePos = 0;
			for (size_t i = 0; i < vectorMonster.size(); ++i)
			{
				if (((MonsterScript*)vectorMonster[i]->GetScripts()[0])->IsMove())
				{
					int pos = ((MonsterScript*)vectorMonster[i]->GetScripts()[0])->GetPosition();
					Vector3 monsterPos = vectorMonster[i]->GetTransform()->GetLocalPos();

					if (((MonsterScript*)vectorMonster[i]->GetScripts()[0])->m_bFirstMove)
					{
						((MonsterScript*)vectorMonster[i]->GetScripts()[0])->m_bFirstMove = false;
						((MonsterScript*)vectorMonster[i]->GetScripts()[0])->m_height = monsterPos.y - 40.f;
					}
					vectorMonster[i]->GetTransform()->SetLocalPos(Vector3(cameraPos.x + (150.f * (battlePos + 1)), ((MonsterScript*)vectorMonster[i]->GetScripts()[0])->m_height, 80.f));
					++battlePos;
				}
			}
		}

		if (m_moveTime < 0.f)
		{
			m_moveTime = 1.f;
			m_bMove = false;
			m_bFirstMove = true;
		}
	}

	if (!g_bOverlap)
	{
		g_bOverlap = true;

		GameObject* hitEffect = SceneManager::GetInst()->FindObjByName(L"MonsterHitEffect");
		if (nullptr != hitEffect && !(hitEffect->IsDead()))
		{
			DeleteObject(hitEffect);
			m_bEffect = true;
		}

		hitEffect = SceneManager::GetInst()->FindObjByName(L"MonsterCriticalHitEffect");
		if (nullptr != hitEffect && !(hitEffect->IsDead()))
		{
			DeleteObject(hitEffect);
			m_bEffect = true;
		}

		GameObject* commonHitEffect = SceneManager::GetInst()->FindObjByName(L"MonsterHitEffect2");
		if (nullptr != commonHitEffect && !(commonHitEffect->IsDead()))
		{
			DeleteObject(commonHitEffect);
			m_bEffect = true;
		}
	}

	if (m_bDebuffDamage)
	{
		m_debuffDamageTime -= DELTATIME;

		if (m_debuffDamageTime < 0.f)
		{
			m_debuffDamageTime = 1.f;
			m_bDebuffDamage = false;

			if (nullptr != m_damageText)
				((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(false);
		}
	}

	if (m_bDamageText)
	{
		SetDamageTextUI();
		m_bDamageText = false;
	}
	
	if (m_bAnimStart)
	{
		m_animTime -= DELTATIME;

		if (m_animTime < 0.f)
		{
			m_animTime = 1.f;
			m_bAnimStart = false;

			if (nullptr != m_damageText)
				((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(false);

			if (m_bTurnStart)
			{
				m_bTurnEnd = true;
			}
		}
	}
	else
	{
		GetAnimator2D()->Play(L"Idle");
	}
}

void MonsterScript::lateUpdate()
{
	if (GetCollider2D()->IsObjectHovered())
	{
		m_bInfoOpen = true;

		if (!Object()->IsDead())
		{
			GameObject* monsterInfo = SceneManager::GetInst()->FindObjByName(L"MonsterInfo");
			if (nullptr != monsterInfo)
			{
				((UIScript*)monsterInfo->GetScripts()[0])->SetTargetScript(this);
			}
		}
	}
	else if(false == GetCollider2D()->IsObjectHovered())
	{
		m_bInfoOpen = false;
	}

	if (GetCollider2D()->IsObjectHovered() && IsMouseClicked())
	{
		m_bSelect = true;

		Scene* curScene = SceneManager::GetInst()->GetCurScene();
		Layer* layer = curScene->GetLayer(0);

		const vector<GameObject*>& vectorObj = layer->GetParentObj();

		for (size_t i = 0; i < vectorObj.size(); ++i)
		{
			if (vectorObj[i]->GetObjectType() == OBJECT_TYPE::PLAYER)
			{
				const vector<Script*>& vectorScript = vectorObj[i]->GetScripts();

				if (((PlayerScript*)vectorScript[0])->IsSelected())
				{
					((PlayerScript*)vectorScript[0])->SetTarget(Object());
				}
			}
		}
	}

	if (nullptr != m_target)
	{
		SkillTrigger(m_target);
		m_target = nullptr;
	}

	if (m_curHP <= 0 && !Object()->IsDead())
	{
		m_curHP = 0;

		if (!m_bAnimStart)
		{
			DeleteObject(Object());

			m_bInfoOpen = false;

			GameObject* monsterInfo = SceneManager::GetInst()->FindObjByName(L"MonsterInfo");
			if (nullptr != monsterInfo)
			{
				((UIScript*)monsterInfo->GetScripts()[0])->SetTargetScript(nullptr);
			}
		}
	}

	if (m_bTurnEnd)
	{
		CalDebuffDamage();
		m_stunTurnCount -= 1;
		m_debuffTurnCount -= 1;
		m_bleedTurnCount -= 1;
		m_blightTurnCount -= 1;
	}

	ReleaseDebuff();
}

void MonsterScript::Hit(int _damage)
{
	m_bHit = true;
	GetAnimator2D()->Play(L"Defend");
	m_curHP -= _damage;
	m_bAnimStart = true;
	m_bMove = true;

	if (g_bHitEffect)
	{
		g_bHitEffect = false;
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 cameraPos = camera->GetTransform()->GetLocalPos();

		wstring buffer;

		if (m_bCritical)
		{
			Ptr<Prefab> hitEffect = ResourceManager::GetInst()->Load<Prefab>(L"MonsterCriticalHitEffect", L"prefab\\MonsterCriticalHitEffect.prefab");
			Vector3 pos = hitEffect->GetPrefabObject()->GetTransform()->GetLocalPos();
			pos.x += cameraPos.x;
			pos.y = cameraPos.y;
			pos.z = 50.f;
			Instantiate(hitEffect, pos, 2);
		}
		else
		{
			Ptr<Prefab> hitEffect = ResourceManager::GetInst()->Load<Prefab>(L"MonsterHitEffect", L"prefab\\MonsterHitEffect.prefab");
			Vector3 pos = hitEffect->GetPrefabObject()->GetTransform()->GetLocalPos();
			pos.x += cameraPos.x;
			pos.y = cameraPos.y;
			pos.z = 50.f;
			Instantiate(hitEffect, pos, 2);
		}

		Ptr<Prefab> commonHitEffect = ResourceManager::GetInst()->Load<Prefab>(L"MonsterHitEffect2", L"prefab\\MonsterHitEffect2.prefab");
		Vector3 effectPos = commonHitEffect->GetPrefabObject()->GetTransform()->GetLocalPos();
		Vector3 pos = GetTransform()->GetLocalPos();
		effectPos.x = pos.x;
		pos.y = cameraPos.y;
		effectPos.z = 50.f;
		Instantiate(commonHitEffect, effectPos, 2);
	}

	InputDamageTextUI(_damage);
	m_bCritical = false;
}

void MonsterScript::SetDebuff(DEBUFF_TYPE_MONSTER _type, UINT _turn, int _damage)
{
	switch (_type)
	{
	case DEBUFF_TYPE_MONSTER::STUN:
	{
		m_debuff |= MONSTER_STATUS_STUN;
		m_stunTurnCount = _turn;
	}
		break;
	case DEBUFF_TYPE_MONSTER::DEBUFF:
	{
		m_debuff |= MONSTER_STATUS_DEBUFF;
		m_debuffTurnCount = _turn;
	}
		break;
	case DEBUFF_TYPE_MONSTER::BLEED:
	{
		m_debuff |= MONSTER_STATUS_BLEED;
		m_bleedTurnCount = _turn;
		m_bleedDamage = _damage;
	}
		break;
	case DEBUFF_TYPE_MONSTER::MOVE:
	{
		m_debuff |= MONSTER_STATUS_MOVE;
	}
		break;
	case DEBUFF_TYPE_MONSTER::BLIGHT:
	{
		m_debuff |= MONSTER_STATUS_BLIGHT;
		m_blightTurnCount = _turn;
		m_blightDamage = _damage;
	}
		break;
	}
}

void MonsterScript::ReleaseDebuff()
{
	if (0 >= m_stunTurnCount)
	{
		m_debuff &= ~MONSTER_STATUS_STUN;
	}

	if (0 >= m_debuffTurnCount)
	{
		m_debuff &= ~MONSTER_STATUS_DEBUFF;
	}

	if (0 >= m_bleedTurnCount)
	{
		m_debuff &= ~MONSTER_STATUS_BLEED;
		m_bleedDamage = 0;
	}

	if (0 >= m_blightTurnCount)
	{
		m_debuff &= ~MONSTER_STATUS_BLIGHT;
		m_blightDamage = 0;
	}
}

void MonsterScript::CalDebuffDamage()
{
	if (m_debuff & MONSTER_STATUS_BLEED)
	{
		m_curHP -= m_bleedDamage;
		InputDamageTextUI(m_bleedDamage);
		m_bDamageText = true;
		m_bDebuffDamage = true;
	}

	if (m_debuff & MONSTER_STATUS_BLIGHT)
	{
		m_curHP -= m_blightDamage;
		InputDamageTextUI(m_blightDamage);
		m_bDamageText = true;
		m_bDebuffDamage = true;
	}
}

void MonsterScript::InputDamageTextUI(int _damage)
{
	wstring buffer = L"DamageText";
	buffer += Object()->GetName();

	if (m_bFirstDamageText)
	{
		Ptr<Prefab> damageText = ResourceManager::GetInst()->Load<Prefab>(L"DamageText", L"prefab\\DamageText.prefab");
		damageText->GetPrefabObject()->SetName(buffer);
		((TextUIScript*)damageText->GetPrefabObject()->GetScripts()[0])->SetDamageNumber(_damage);
		m_damageText = damageText->GetPrefabObject()->Clone();
		((TextUIScript*)m_damageText->GetScripts()[0])->SetTargetType(TEXT_TARGET_TYPE::GENERAL);
		((TextUIScript*)m_damageText->GetScripts()[0])->SetUsage(TEXT_USAGE::GENERAL);
		CreateObject(m_damageText, Vector3(0.f, 0.f, 100.f), 20);
		m_bFirstDamageText = false;
	}
	else
	{
		((TextUIScript*)m_damageText->GetScripts()[0])->SetDamageNumber(_damage);
		((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(true);
	}

	m_bDamageText = true;
}

void MonsterScript::SetDamageTextUI()
{
	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
	Vector3 cameraPos = camera->GetTransform()->GetLocalPos();
	Vector3 pos = GetTransform()->GetLocalPos();
	Vector2 resolution = Core::GetInst()->GetWindowResolution();

	if (nullptr != m_damageText)
	{
		((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(true);
		((TextUIScript*)m_damageText->GetScripts()[0])->SetFontSize(25.f);

		if (m_bDodge)
		{
			((TextUIScript*)m_damageText->GetScripts()[0])->SetText(L"회피!");
			m_bDodge = false;
		}
		else
		{
			int damage = ((TextUIScript*)m_damageText->GetScripts()[0])->GetDamageNumber();
			wchar_t buffer[255] = L"";
			if (m_bCritical)
			{

				swprintf_s(buffer, L"치명타!\n%d", damage);
				((TextUIScript*)m_damageText->GetScripts()[0])->SetText(buffer);
			}
			else
			{
				swprintf_s(buffer, L"\n%d", damage);
				((TextUIScript*)m_damageText->GetScripts()[0])->SetText(buffer);
			}
		}

		pos.x = pos.x - cameraPos.x + (resolution.x / 2.f);

		((TextUIScript*)m_damageText->GetScripts()[0])->SetTextPos(Vector2(pos.x, 100.f));
		((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(true);
	}
}

bool MonsterScript::IsStun()
{
	return m_debuff & MONSTER_STATUS_STUN;
}

void MonsterScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_maxHP, _file);
	SaveToFile(&m_type, _file);
	SaveToFile(&m_dodge, _file);
	SaveToFile(&m_protection, _file);
	SaveToFile(&m_speed, _file);
	SaveToFile(&m_stunResistance, _file);
	SaveToFile(&m_debuffResistance, _file);
	SaveToFile(&m_bleedResistance, _file);
	SaveToFile(&m_moveResistance, _file);
	SaveToFile(&m_blightResistance, _file);
}

void MonsterScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_maxHP, _file);
	LoadFromFile(&m_type, _file);
	LoadFromFile(&m_dodge, _file);
	LoadFromFile(&m_protection, _file);
	LoadFromFile(&m_speed, _file);
	LoadFromFile(&m_stunResistance, _file);
	LoadFromFile(&m_debuffResistance, _file);
	LoadFromFile(&m_bleedResistance, _file);
	LoadFromFile(&m_moveResistance, _file);
	LoadFromFile(&m_blightResistance, _file);
}