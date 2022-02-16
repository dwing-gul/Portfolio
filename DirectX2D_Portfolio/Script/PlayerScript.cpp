#include "pch.h"
#include "PlayerScript.h"
#include "MonsterScript.h"
#include "SkillButtonScript.h"
#include "TextUIScript.h"
#include "LightScript.h"
#include "WallScript.h"
#include <Engine/Core.h>
#include <Engine/KeyManager.h>
#include <Engine/TimeManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/MeshRender.h>
#include <Engine/Transform.h>
#include <Engine/Animator2D.h>
#include <Engine/Collider2D.h>
#include <Engine/Camera.h>
#include <Engine/GameObject.h>

#define PLAYER_STATUS_STUN			1
#define PLAYER_STATUS_DEBUFF		2
#define PLAYER_STATUS_BLEED			4
#define PLAYER_STATUS_MOVE			8
#define PLAYER_STATUS_BLIGHT		16
#define PLAYER_STATUS_DISEASE		32
#define PLAYER_STATUS_DEATHBLOW		64

GameObject* PlayerScript::m_selectPlayer = nullptr;

PlayerScript::PlayerScript(SCRIPT_TYPE _type) :
	Script((UINT)_type),
	m_prevObj(nullptr),
	m_nextObj(nullptr),
	m_target(nullptr),
	m_damageText(nullptr),
	m_bSelect(false),
	m_skillType(SKILL_TYPE::END),
	m_position(0),
	m_moveSpeed(300.f),
	m_maxHP(10),
	m_curHP(0),
	m_maxStress(200),
	m_curStress(0),
	m_dodge(1),
	m_protection(10),
	m_speed(1),
	m_turnDecision(1),
	m_accuracyModifier(10),
	m_criticalHitChance(10),
	m_damage(POINT{ 1, 2 }),
	m_virtueChance(25),
	m_stunResistance(20),
	m_debuffResistance(20),
	m_bleedResistance(10),
	m_moveResistance(20),
	m_blightResistance(20),
	m_diseaseResistance(20),
	m_deathblowResistance(67),
	m_trapRelease(0),
	m_bHit(false),
	m_bCritical(false),
	m_bDodge(false),
	m_bLeftCollision(false),
	m_bRightCollision(false),
	m_bBattlePhase(false),
	m_bTurnStart(false),
	m_bTurnEnd(false),
	m_bAnimStart(false),
	m_bMove(false),
	m_bFirstMove(true),
	m_bFirst(true),
	m_bEffect(true),
	m_bAddStress(false),
	m_bHeal(false),
	m_bFirstDamageText(true),
	m_bDamageText(false),
	m_bDebuffDamage(false),
	m_bHealTarget(false),
	m_animTime(1.f),
	m_moveTime(1.f),
	m_stressTime(0.5f),
	m_debuffDamageTime(0.5f),
	m_height(0.f),
	m_term(5.f),
	m_debuff(0),
	m_stunTurnCount(0),
	m_debuffTurnCount(0),
	m_bleedTurnCount(0),
	m_blightTurnCount(0),
	m_diseaseTurnCount(0),
	m_bleedDamage(0),
	m_blightDamage(0)
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::start()
{
	m_curHP = m_maxHP;

	Ptr<Prefab> selectEffect = ResourceManager::GetInst()->Load<Prefab>(L"SelectEffect", L"prefab\\SelectEffect.prefab");
	GameObject* clone = selectEffect->GetPrefabObject()->Clone();
	AddChild(Object(), clone);
}

void PlayerScript::update()
{
	if (KEY_TAP(KEY::P))
	{
		m_curHP = 10000;
		m_damage = POINT{ 100, 100 };
	}

	const vector<GameObject*>& vectorWall = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::INTERACTION);

	GameObject* rightWall = nullptr;
	GameObject* leftWall = nullptr;

	for (size_t i = 0; i < vectorWall.size(); ++i)
	{
		const vector<Script*> vectorWallScript = vectorWall[i]->GetScripts();

		for (size_t j = 0; j < vectorWallScript.size(); ++j)
		{
			if (vectorWallScript[j]->GetScriptType() == (UINT)SCRIPT_TYPE::WALLSCRIPT)
			{
				if (abs(GetTransform()->GetLocalPos().y - vectorWall[i]->GetTransform()->GetLocalPos().y) < 200.f)
				{
					if (((WallScript*)vectorWallScript[j])->GetWallType() == WALL_TYPE::RIGHT)
					{
						rightWall = vectorWall[i];
					}
					else if (((WallScript*)vectorWallScript[j])->GetWallType() == WALL_TYPE::LEFT)
					{
						leftWall = vectorWall[i];
					}
				}
			}
		}
	}

	float xHalfRightWall = 0.f;
	float xHalfLeftWall = 0.f;

	if(nullptr != rightWall)
		xHalfRightWall = (rightWall->GetTransform()->GetLocalScale().x * rightWall->GetCollider2D()->GetOffsetScale().x) / 2.f;

	if (nullptr != leftWall)
		xHalfLeftWall = (leftWall->GetTransform()->GetLocalScale().x * leftWall->GetCollider2D()->GetOffsetScale().x) / 2.f;
	float xHalf = (GetTransform()->GetLocalScale().x * GetCollider2D()->GetOffsetScale().x) / 2.f;

	bool collsion = GetCollider2D()->IsCollision();
	if (GetCollider2D()->IsCollision())
	{
		float rightCheckX = 0.f;

		if (nullptr != rightWall)
			rightCheckX = (rightWall->GetTransform()->GetLocalPos().x - xHalfRightWall) - (GetTransform()->GetLocalPos().x + xHalf);

		float leftCheckX = 0.f;

		if (nullptr != leftWall)
			leftCheckX = (GetTransform()->GetLocalPos().x - xHalf) - (leftWall->GetTransform()->GetLocalPos().x + xHalfLeftWall);

		if (nullptr != leftWall)
		{
			if (leftCheckX < 0.f)
				m_bLeftCollision = true;
			else
				m_bLeftCollision = false;
		}

		if (nullptr != rightWall)
		{
			if (rightCheckX < 0.f)
				m_bRightCollision = true;
			else
				m_bRightCollision = false;
		}
	}
	else
	{
		m_bLeftCollision = false;
		m_bRightCollision = false;
	}

	if (m_curHP <= 0 && !Object()->IsDead())
	{
		m_curHP = 0;

		if (!m_bAnimStart)
		{
			DeleteObject(Object());
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

	if (m_bSelect)
	{
		if (m_selectPlayer != Object())
		{
			m_bSelect = false;
			return;
		}

		GameObject* skillButton1 = SceneManager::GetInst()->FindObjByName(L"SkillButton_1");
		GameObject* skillButton2 = SceneManager::GetInst()->FindObjByName(L"SkillButton_2");
		GameObject* skillButton3 = SceneManager::GetInst()->FindObjByName(L"SkillButton_3");
		GameObject* skillButton4 = SceneManager::GetInst()->FindObjByName(L"SkillButton_4");
		GameObject* skillButton_Move = SceneManager::GetInst()->FindObjByName(L"SkillButton_Move");
		GameObject* skillButton_Pass = SceneManager::GetInst()->FindObjByName(L"SkillButton_Pass");
		GameObject* playerPortrait = SceneManager::GetInst()->FindObjByName(L"PlayerPortrait");

		if (nullptr == skillButton1 || nullptr == skillButton2 || nullptr == skillButton3 || nullptr == skillButton4 ||
			nullptr == skillButton_Move || nullptr == skillButton_Pass || nullptr == playerPortrait)
			return;

		SkillSetSetting(skillButton1);
		SkillSetSetting(skillButton2);
		SkillSetSetting(skillButton3);
		SkillSetSetting(skillButton4);
		SkillSetSetting(skillButton_Move);
		SkillSetSetting(skillButton_Pass);
		SkillSetSetting(playerPortrait);
	}
}

void PlayerScript::lateUpdate()
{
	if (!m_bBattlePhase)
	{
		Vector3 pos = GetTransform()->GetLocalPos();

		if (KEY_HOLD(KEY::D) && !m_bRightCollision)
		{
			pos.x += m_moveSpeed * DELTATIME;
		}

		if (KEY_HOLD(KEY::A) && !m_bLeftCollision)
		{
			pos.x -= m_moveSpeed * 0.7f * DELTATIME;
		}

		if (KEY_AWAY(KEY::D) || KEY_AWAY(KEY::A))
		{
			GetAnimator2D()->Play(L"Idle");
		}

		if (KEY_TAP(KEY::D) || KEY_TAP(KEY::A))
		{
			GetAnimator2D()->Play(L"Walk");
		}

		GetTransform()->SetLocalPos(pos);

		if (m_bAnimStart)
		{
			m_animTime = 1.f;
			m_bAnimStart = false;
			GetAnimator2D()->Play(L"Idle");
		}

		wstring buffer = L"PlayerHitEffect";
		buffer += Object()->GetName();

		GameObject* hitEffect = SceneManager::GetInst()->FindObjByName(buffer);
		if (nullptr != hitEffect && !(hitEffect->IsDead()))
		{
			DeleteObject(hitEffect);
		}

		buffer = L"PlayerCriticalHitEffect";
		buffer += Object()->GetName();

		hitEffect = SceneManager::GetInst()->FindObjByName(buffer);
		if (nullptr != hitEffect && !(hitEffect->IsDead()))
		{
			DeleteObject(hitEffect);
		}

		buffer = L"PlayerHitEffect2";
		buffer += Object()->GetName();

		GameObject* commonHitEffect = SceneManager::GetInst()->FindObjByName(buffer);
		if (nullptr != commonHitEffect && !(commonHitEffect->IsDead()))
		{
			DeleteObject(commonHitEffect);
		}

		if (!m_bAddStress)
		{
			Ptr<Prefab> stressEffect;
			buffer = L"StressEffect";
			buffer += Object()->GetName();
			GameObject* effect = SceneManager::GetInst()->FindObjByName(buffer);
			if (nullptr != effect && !(effect->IsDead()))
			{
				DeleteObject(effect);
			}
		}

		m_animTime = 1.f;
		m_bAnimStart = false;

		m_bEffect = true;

		GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
		if (0.f >= ((LightScript*)light->GetScripts()[0])->GetLightRatio())
		{
			m_term -= DELTATIME;

			if (m_term < 0.f)
			{
				m_term = 5.f;
				m_bAddStress = true;
			}
		}

		if (m_bAddStress)
		{
			m_stressTime -= DELTATIME;

			Ptr<Prefab> stressEffect;
			buffer = L"StressEffect";
			buffer += Object()->GetName();

			if (m_bFirst)
			{
				m_bFirst = false;
				AddStress(4);

				stressEffect = ResourceManager::GetInst()->Load<Prefab>(L"StressEffect", L"prefab\\StressCloudEffect.prefab");
				stressEffect->GetPrefabObject()->SetName(buffer);
				Vector3 pos = GetTransform()->GetLocalPos();
				pos.y = 40.f;
				pos.z = 40.f;
				Instantiate(stressEffect, pos, 2);

				Ptr<Sound> stress_up = ResourceManager::GetInst()->Load<Sound>(L"Sound_Stress_Up", L"sound\\general\\gen_status_stress_up_comp.wav");
				stress_up->Play(1, 0.5f, true);
			}

			GameObject* effect = SceneManager::GetInst()->FindObjByName(buffer);

			if (m_stressTime < 0.f)
			{
				m_stressTime = 0.5f;
				m_bAddStress = false;

				if (nullptr != effect && !(effect->IsDead()))
					DeleteObject(effect);

				m_bFirst = true;
			}
		}
	}
	else
	{
		m_term = 5.f;
		if (m_bDodge)
		{
			m_bMove = true;
			m_bAnimStart = true;
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

			vector<GameObject*> vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
			for (size_t i = 0; i < vectorPlayer.size(); ++i)
			{
				if (((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->IsMove())
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
					m_height = pos.y + 20.f;
				}

				GetTransform()->SetLocalPos(Vector3(cameraPos.x - 150.f, m_height, 80.f));
			}
			else if (count > 1)
			{
				int battlePos = 0;
				for (size_t i = 0; i < vectorPlayer.size(); ++i)
				{
					if (((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->IsMove())
					{
						int pos = ((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->GetPosition();
						Vector3 playerPos = vectorPlayer[i]->GetTransform()->GetLocalPos();

						if (((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->m_bFirstMove)
						{
							((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->m_bFirstMove = false;
							((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->m_height = playerPos.y + 20.f;
						}
						vectorPlayer[i]->GetTransform()->SetLocalPos(Vector3(cameraPos.x - (150.f * (battlePos + 1)), ((PlayerScript*)vectorPlayer[i]->GetScripts()[0])->m_height, 80.f));
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

		if (m_bAddStress)
		{
			m_stressTime -= DELTATIME;

			Ptr<Prefab> stressEffect;
			wstring buffer = L"StressEffect";
			buffer += Object()->GetName();

			if (m_bFirst)
			{
				m_bFirst = false;

				stressEffect = ResourceManager::GetInst()->Load<Prefab>(L"StressEffect", L"prefab\\StressCloudEffect.prefab");
				stressEffect->GetPrefabObject()->SetName(buffer);
				Vector3 pos = GetTransform()->GetLocalPos();
				pos.y = 40.f;
				pos.z = 40.f;
				Instantiate(stressEffect, pos, 2);
			}

			GameObject* effect = SceneManager::GetInst()->FindObjByName(buffer);

			if (m_stressTime < 0.f)
			{
				m_stressTime = 0.5f;
				m_bAddStress = false;

				if (nullptr != effect && !(effect->IsDead()))
					DeleteObject(effect);

				m_bFirst = true;
			}
		}

		if (m_bDamageText)
		{
			SetDamageTextUI();
			m_bDamageText = false;
		}

		if(!m_bAnimStart)
			GetAnimator2D()->Play(L"Battle");
		else
		{
			m_animTime -= DELTATIME;

			if (m_animTime < 0.f)
			{
				m_animTime = 1.f;
				m_bAnimStart = false;

				wstring buffer = L"PlayerHitEffect";
				buffer += Object()->GetName();

				GameObject* hitEffect = SceneManager::GetInst()->FindObjByName(buffer);
				if (nullptr != hitEffect && !(hitEffect->IsDead()))
				{
					DeleteObject(hitEffect);
					m_bEffect = true;
				}

				buffer = L"PlayerCriticalHitEffect";
				buffer += Object()->GetName();

				hitEffect = SceneManager::GetInst()->FindObjByName(buffer);
				if (nullptr != hitEffect && !(hitEffect->IsDead()))
				{
					DeleteObject(hitEffect);
					m_bEffect = true;
				}

				buffer = L"PlayerHitEffect2";
				buffer += Object()->GetName();

				GameObject* commonHitEffect = SceneManager::GetInst()->FindObjByName(buffer);
				if (nullptr != commonHitEffect && !(commonHitEffect->IsDead()))
				{
					DeleteObject(commonHitEffect);
					m_bEffect = true;
				}

				if (nullptr != m_damageText)
				{
					((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(false);
				}

				if(m_bTurnStart)
					m_bTurnEnd = true;
			}
		}
	}

	if (GetCollider2D()->IsObjectHovered() && IsMouseClicked())
	{
		m_bSelect = true;

		if(!m_bBattlePhase)
			m_selectPlayer = Object();
		else
		{
			if (m_selectPlayer != Object())
			{
				Scene* curScene = SceneManager::GetInst()->GetCurScene();
				Layer* layer = curScene->GetLayer(0);

				const vector<GameObject*>& vectorObj = layer->GetParentObj();

				for (size_t i = 0; i < vectorObj.size(); ++i)
				{
					if (vectorObj[i]->GetObjectType() == OBJECT_TYPE::PLAYER)
					{
						const vector<Script*>& vectorScript = vectorObj[i]->GetScripts();

						if (((PlayerScript*)vectorScript[0])->IsSelected() && vectorObj[i] != Object())
						{
							((PlayerScript*)vectorScript[0])->SetTarget(Object());
						}
					}
				}
			}
			else
			{
				SetTarget(Object());
			}
		}
	}

	if (m_bTurnEnd)
	{
		MonsterScript::g_bOverlap = false;
		MonsterScript::g_bHitEffect = true;

		if (m_curStress == 200)
		{
			m_curHP -= 1;
		}

		GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
		if (0.f >= ((LightScript*)light->GetScripts()[0])->GetLightRatio())
		{
			AddStress(4);
		}

		CalDebuffDamage();
		m_stunTurnCount -= 1;
		m_debuffTurnCount -= 1;
		m_bleedTurnCount -= 1;
		m_blightTurnCount -= 1;
		m_diseaseTurnCount -= 1;
	}

	ReleaseDebuff();
}

void PlayerScript::Hit(int _damage)
{
	m_bHit = true;
	GetAnimator2D()->Play(L"Defend");
	m_curHP -= _damage;
	m_bAnimStart = true;
	m_bMove = true;

	// 피격 이펙트;
	if (m_bEffect)
	{
		m_bEffect = false;
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 cameraPos = camera->GetTransform()->GetLocalPos();
		
		wstring buffer;

		if (m_bCritical)
		{
			buffer = L"PlayerCriticalHitEffect";
			buffer += Object()->GetName();

			Ptr<Prefab> hitEffect = ResourceManager::GetInst()->Load<Prefab>(L"PlayerCriticalHitEffect", L"prefab\\PlayerCriticalHitEffect.prefab");
			hitEffect->GetPrefabObject()->SetName(buffer);
			Vector3 pos = hitEffect->GetPrefabObject()->GetTransform()->GetLocalPos();
			pos.x += cameraPos.x;
			pos.y = cameraPos.y;
			pos.z = 50.f;
			Instantiate(hitEffect, pos, 2);
		}
		else
		{
			buffer = L"PlayerHitEffect";
			buffer += Object()->GetName();

			Ptr<Prefab> hitEffect = ResourceManager::GetInst()->Load<Prefab>(L"PlayerHitEffect", L"prefab\\PlayerHitEffect.prefab");
			hitEffect->GetPrefabObject()->SetName(buffer);
			Vector3 pos = hitEffect->GetPrefabObject()->GetTransform()->GetLocalPos();
			pos.x += cameraPos.x;
			pos.y = cameraPos.y;
			pos.z = 50.f;
			Instantiate(hitEffect, pos, 2);
		}

		buffer = L"PlayerHitEffect2";
		buffer += Object()->GetName();

		Ptr<Prefab> commonHitEffect = ResourceManager::GetInst()->Load<Prefab>(L"PlayerHitEffect2", L"prefab\\PlayerHitEffect2.prefab");
		commonHitEffect->GetPrefabObject()->SetName(buffer);
		Vector3 effectPos = commonHitEffect->GetPrefabObject()->GetTransform()->GetLocalPos();
		Vector3 pos = GetTransform()->GetLocalPos();
		effectPos.x = pos.x;
		effectPos.y = cameraPos.y;
		effectPos.z = 50.f;
		Instantiate(commonHitEffect, effectPos, 2);

		InputDamageTextUI(_damage);
		m_bCritical = false;
	}
}

void PlayerScript::SetDebuff(DEBUFF_TYPE_PLAYER _type, UINT _turn, int _damage)
{
	switch (_type)
	{
	case DEBUFF_TYPE_PLAYER::STUN:
	{
		m_debuff |= PLAYER_STATUS_STUN;
		m_stunTurnCount = _turn;
	}
		break;
	case DEBUFF_TYPE_PLAYER::DEBUFF:
	{
		m_debuff |= PLAYER_STATUS_DEBUFF;
		m_debuffTurnCount = _turn;
	}
		break;
	case DEBUFF_TYPE_PLAYER::BLEED:
	{
		m_debuff |= PLAYER_STATUS_BLEED;
		m_bleedTurnCount = _turn;
		m_bleedDamage = _damage;
	}
		break;
	case DEBUFF_TYPE_PLAYER::MOVE:
	{
		m_debuff |= PLAYER_STATUS_MOVE;
	}
		break;
	case DEBUFF_TYPE_PLAYER::BLIGHT:
	{
		m_debuff |= PLAYER_STATUS_BLIGHT;
		m_blightTurnCount = _turn;
		m_blightDamage = _damage;
	}
		break;
	case DEBUFF_TYPE_PLAYER::DISEASE:
	{
		m_debuff |= PLAYER_STATUS_DISEASE;
		m_diseaseTurnCount = _turn;
	}
		break;
	case DEBUFF_TYPE_PLAYER::DEATHBLOW:
	{
		m_debuff |= PLAYER_STATUS_DEATHBLOW;
	}
		break;
	}
}

void PlayerScript::ReleaseDebuffByType(UINT _type)
{
	if ((UINT)DEBUFF_TYPE_PLAYER::STUN & _type)
	{
		m_debuff &= ~PLAYER_STATUS_STUN;
	}
	if ((UINT)DEBUFF_TYPE_PLAYER::BLEED & _type)
	{
		m_debuff &= ~PLAYER_STATUS_BLEED;
		m_bleedDamage = 0;
	}
	if ((UINT)DEBUFF_TYPE_PLAYER::BLIGHT & _type)
	{
		m_debuff &= ~PLAYER_STATUS_BLIGHT;
		m_blightDamage = 0;
	}
	if ((UINT)DEBUFF_TYPE_PLAYER::DEATHBLOW & _type)
	{
		m_debuff &= ~PLAYER_STATUS_DEATHBLOW;
	}
	if ((UINT)DEBUFF_TYPE_PLAYER::DISEASE & _type)
	{
		m_debuff &= ~PLAYER_STATUS_DISEASE;
	}
	if ((UINT)DEBUFF_TYPE_PLAYER::DEBUFF & _type)
	{
		m_debuff &= ~PLAYER_STATUS_DEBUFF;
	}
}

void PlayerScript::ReleaseDebuff()
{
	if (0 >= m_stunTurnCount)
	{
		m_debuff &= ~PLAYER_STATUS_STUN;
	}

	if (0 >= m_debuffTurnCount)
	{
		m_debuff &= ~PLAYER_STATUS_DEBUFF;
	}

	if (0 >= m_bleedTurnCount)
	{
		m_debuff &= ~PLAYER_STATUS_BLEED;
	}

	if (0 >= m_blightTurnCount)
	{
		m_debuff &= ~PLAYER_STATUS_BLIGHT;
	}

	if (0 >= m_diseaseTurnCount)
	{
		m_debuff &= ~PLAYER_STATUS_DISEASE;
	}
}

void PlayerScript::CalDebuffDamage()
{
	if (m_debuff & PLAYER_STATUS_BLEED)
	{
		m_curHP -= m_bleedDamage;
		InputDamageTextUI(m_bleedDamage);
		m_bDebuffDamage = true;
		m_bDamageText = true;
	}

	if (m_debuff & PLAYER_STATUS_BLIGHT)
	{
		m_curHP -= m_blightDamage;
		InputDamageTextUI(m_blightDamage);
		m_bDebuffDamage = true;
		m_bDamageText = true;
	}
}

void PlayerScript::AddStress(int _stress)
{
	m_curStress += _stress;
	if (m_curStress > m_maxStress)
		m_curStress = m_maxStress;

	m_bAddStress = true;
}

void PlayerScript::SkillSetSetting(GameObject* _skillButton)
{
	const vector<Script*>& vectorScript = _skillButton->GetScripts();

	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if ((UINT)SCRIPT_TYPE::SKILLBUTTONSCRIPT == vectorScript[i]->GetScriptType())
		{
			((SkillButtonScript*)vectorScript[i])->SetOwnerName(Object()->GetName());

			if (((SkillButtonScript*)vectorScript[i])->GetPosition() < m_skillSet.size())
				((SkillButtonScript*)vectorScript[i])->SetType(m_skillSet[((SkillButtonScript*)vectorScript[i])->GetPosition()]);
		}
	}
}

void PlayerScript::InputDamageTextUI(int _damage)
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
		CreateObject(m_damageText, Vector3(0.f, 0.f, 100.f), 21);
		m_bFirstDamageText = false;
	}
	else
	{
		((TextUIScript*)m_damageText->GetScripts()[0])->SetDamageNumber(_damage);
		((TextUIScript*)m_damageText->GetScripts()[0])->SetShow(true);
	}

	m_bDamageText = true;
}

void PlayerScript::SetDamageTextUI()
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

bool PlayerScript::IsStun()
{
	return m_debuff & PLAYER_STATUS_STUN;
}

void PlayerScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_maxHP, _file);
	SaveToFile(&m_dodge, _file);
	SaveToFile(&m_protection, _file);
	SaveToFile(&m_speed, _file);
	SaveToFile(&m_turnDecision, _file);
	SaveToFile(&m_accuracyModifier, _file);
	SaveToFile(&m_criticalHitChance, _file);
	SaveToFile(&m_damage, _file);
	SaveToFile(&m_virtueChance, _file);
	SaveToFile(&m_stunResistance, _file);
	SaveToFile(&m_debuffResistance, _file);
	SaveToFile(&m_bleedResistance, _file);
	SaveToFile(&m_moveResistance, _file);
	SaveToFile(&m_blightResistance, _file);
	SaveToFile(&m_diseaseResistance, _file);
	SaveToFile(&m_deathblowResistance, _file);
	SaveToFile(&m_trapRelease, _file);
}

void PlayerScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_maxHP, _file);
	LoadFromFile(&m_dodge, _file);
	LoadFromFile(&m_protection, _file);
	LoadFromFile(&m_speed, _file);
	LoadFromFile(&m_turnDecision, _file);
	LoadFromFile(&m_accuracyModifier, _file);
	LoadFromFile(&m_criticalHitChance, _file);
	LoadFromFile(&m_damage, _file);
	LoadFromFile(&m_virtueChance, _file);
	LoadFromFile(&m_stunResistance, _file);
	LoadFromFile(&m_debuffResistance, _file);
	LoadFromFile(&m_bleedResistance, _file);
	LoadFromFile(&m_moveResistance, _file);
	LoadFromFile(&m_blightResistance, _file);
	LoadFromFile(&m_diseaseResistance, _file);
	LoadFromFile(&m_deathblowResistance, _file);
	LoadFromFile(&m_trapRelease, _file);
}
