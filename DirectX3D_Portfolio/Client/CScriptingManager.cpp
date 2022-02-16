#include "pch.h"
#include "CScriptingManager.h"
#include <Script/define.h>
#include <Engine/CGameObject.h>
#include <Engine/CSceneManager.h>
#include <Engine/CRenderManager.h>

#include <Script/CItemManager.h>
#include <Script/CPlayerScript.h>
#include <Script/CFontUIScript.h>
#include <Script/CSkillUIScript.h>
#include <Script/CBarUIScript.h>
#include <Script/CButtonUIScript.h>
#include <Script/CLightScript.h>
#include <Script/CMonsterManager.h>

bool bEnd = false;

CScriptingManager::CScriptingManager() :
	m_gameTime(0.f),
	m_totalTime(0.f),
	m_bDayChange(false),
	m_DayFont(nullptr),
	m_Day_N_Night_Font(nullptr),
	m_dayIcon(nullptr),
	m_light(nullptr),
	m_levelFont(nullptr),
	m_hpBar(nullptr),
	m_spBar(nullptr),
	m_hpBarFont(nullptr),
	m_spBarFont(nullptr),
	m_expBar(nullptr),
	m_AttackPowerFont(nullptr),
	m_DefenseFont(nullptr),
	m_ExtraAttackFont(nullptr),
	m_DMGReductionFont(nullptr),
	m_SkillDMGIncFont(nullptr),
	m_SkillDMGReductionFont(nullptr),
	m_MoveSpeedFont(nullptr),
	m_AttackSpeedFont(nullptr)
{
}

CScriptingManager::~CScriptingManager()
{
}

void CScriptingManager::init()
{
	CItemManager::GetInst()->init();
	CMonsterManager::GetInst()->init();
}

void CScriptingManager::progress()
{
	if (SCENE_MODE::STOP == CSceneManager::GetInst()->GetSceneMode())
	{
		m_gameTime = 0.f;
		m_totalTime = 0.f;
		m_bDayChange = false;
		m_DayFont = nullptr;
		m_Day_N_Night_Font = nullptr;
		m_dayIcon = nullptr;
		m_light = nullptr;
		m_levelFont = nullptr;
		m_hpBar = nullptr;
		m_spBar = nullptr;
		m_hpBarFont = nullptr;
		m_spBarFont = nullptr;
		m_expBar = nullptr;
		m_AttackPowerFont = nullptr;
		m_DefenseFont = nullptr;
		m_ExtraAttackFont = nullptr;
		m_DMGReductionFont = nullptr;
		m_SkillDMGIncFont = nullptr;
		m_SkillDMGReductionFont = nullptr;
		m_MoveSpeedFont = nullptr;
		m_AttackSpeedFont = nullptr;
	}

	PlayerCountFont();
	DayNTimeFont();
	PlayerLevelFont();
	PlayerBarRatio();
	PlayerStatFont();
	TrackingBarRatio();

	CMonsterManager::GetInst()->SetTotalTime(m_totalTime);
	CMonsterManager::GetInst()->update();
	
}

void CScriptingManager::PlayerCountFont()
{
	vector<CGameObject*> vecPlayer = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);

	CGameObject* playerCountTextUI = CSceneManager::GetInst()->FindObjByName(L"UserNumFont");
	int playerCount = 0;

	for (size_t i = 0; i < vecPlayer.size(); ++i)
	{
		if (((CPlayerScript*)vecPlayer[i]->GetScripts()[0])->IsCharacterDead())
			continue;

		++playerCount;
	}

	if (KEY_TAP(KEY::N) && SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode())
	{
		for (size_t i = 0; i < vecPlayer.size(); ++i)
		{
			if (vecPlayer[i]->GetName() != L"Player")
			{
				((CPlayerScript*)vecPlayer[i]->GetScripts()[0])->Hit(ATTACK_TYPE::BASE, 9999999.f, 1.f, 0.f, 0.f, nullptr);
			}
		}

		CTimeManager::GetInst()->Pause();

		bEnd = true;
	}

	// 살아남은 플레이어 숫자 
	if (nullptr != playerCountTextUI)
		((CFontUIScript*)playerCountTextUI->GetScripts()[0])->SetNumberText(playerCount);
}

void CScriptingManager::DayNTimeFont()
{
	if (nullptr == m_DayFont)
		m_DayFont = CSceneManager::GetInst()->FindObjByName(L"DayCountFont");

	if (nullptr == m_Day_N_Night_Font)
		m_Day_N_Night_Font = CSceneManager::GetInst()->FindObjByName(L"TimeFont");

	if (nullptr == m_dayIcon)
		m_dayIcon = CSceneManager::GetInst()->FindObjByName(L"DayIcon");

	if (nullptr == m_light)
		m_light = CSceneManager::GetInst()->FindObjByName(L"Light3D");

	if (SCENE_MODE::PLAY == CSceneManager::GetInst()->GetSceneMode())
	{
		int day = 1; // 몇일차인지 알려주는 변수
		static bool bDay = true; // 낮인지 밤인지 알려주는 변수

		// 낮/밤 시간
		if (m_gameTime > 150.f)
		{
			m_gameTime = 0.f;
			bDay = bDay == true ? false : true;
		}

		int prevDay = day;
		day = ((int)m_totalTime / 300) + 1;

		if (prevDay != day)
			m_bDayChange = true;

		if (!bEnd)
		{
			m_gameTime += DELTATIME;
			m_totalTime += DELTATIME;
		}

		if (nullptr != m_dayIcon)
		{
			((CButtonUIScript*)m_dayIcon->GetScripts()[0])->SetDay(bDay);
		}

		if (nullptr != m_DayFont)
		{
			((CFontUIScript*)m_DayFont->GetScripts()[0])->SetNumberText(day);
		}

		if (nullptr != m_Day_N_Night_Font)
		{
			((CFontUIScript*)m_Day_N_Night_Font->GetScripts()[0])->SetTime((int)m_gameTime);
		}

		((CLightScript*)m_light->GetScripts()[0])->SetDay(bDay);
		((CLightScript*)m_light->GetScripts()[0])->SetRatio(m_gameTime / 150.f);
	}
	else
	{
		m_gameTime = 0.f;
		m_totalTime = 0.f;
	}
}

void CScriptingManager::PlayerLevelFont()
{
	if (nullptr == m_levelFont)
	{
		const vector<CGameObject*>& vecLevelFont = CSceneManager::GetInst()->FindObjByName_Vector(L"LevelFont");

		for (size_t i = 0; i < vecLevelFont.size(); ++i)
		{
			if (vecLevelFont[i]->GetParent()->GetName() == L"RootUI")
			{
				m_levelFont = vecLevelFont[i];
				break;
			}
		}
	}
	CGameObject* owner	= nullptr;

	if (nullptr != m_levelFont)
	{
		owner = CSceneManager::GetInst()->FindObjByName(L"Player");
	}
	int level;

	// 플레이어 레벨을 띄우는 폰트
	if (nullptr != owner)
	{
		level = ((CPlayerScript*)owner->GetScripts()[0])->GetLevel();
	}

	if (nullptr != m_levelFont)
	{
		((CFontUIScript*)m_levelFont->GetScripts()[0])->SetNumberText(level);
	}
}

void CScriptingManager::PlayerBarRatio()
{
	if (nullptr == m_hpBar)
		m_hpBar = CSceneManager::GetInst()->FindObjByName(L"HPBar");
	if (nullptr == m_spBar)
		m_spBar = CSceneManager::GetInst()->FindObjByName(L"SPBar");
	if (nullptr == m_hpBarFont)
		m_hpBarFont = CSceneManager::GetInst()->FindObjByName(L"HPFont");
	if (nullptr == m_spBarFont)
		m_spBarFont = CSceneManager::GetInst()->FindObjByName(L"SPFont");
	if (nullptr == m_expBar)
		m_expBar = CSceneManager::GetInst()->FindObjByName(L"EXPBar");

	CGameObject* owner = nullptr;

	if (nullptr != m_hpBar)
	{
		owner = CSceneManager::GetInst()->FindObjByName(L"Player");
	}

	float hpRatio = 0.f;
	float spRatio = 0.f;

	float curHP = 0.f, maxHP = 0.f, curSP = 0.f, maxSP = 0.f;
	float curExp = 0.f, maxExp = 0.f, expRatio = 0.f;

	// 플레이어 레벨을 띄우는 폰트
	
	if (nullptr != owner)
	{
		hpRatio = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerHPRatio();
		spRatio = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerSPRatio();
		expRatio = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerExpRatio();

		curHP = ((CPlayerScript*)owner->GetScripts()[0])->GetCurHP();
		maxHP = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerMaxHP();

		curSP = ((CPlayerScript*)owner->GetScripts()[0])->GetCurSP();
		maxSP = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerMaxSP();

		curExp = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerCurExp();
		maxExp = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerMaxExp();
	}

	if (nullptr != m_hpBar)
	{
		((CBarUIScript*)m_hpBar->GetScripts()[0])->SetRatio(hpRatio);
	}

	if (nullptr != m_spBar)
	{
		((CBarUIScript*)m_spBar->GetScripts()[0])->SetRatio(spRatio);
	}

	if (nullptr != m_expBar)
	{
		((CBarUIScript*)m_expBar->GetScripts()[0])->SetRatio(expRatio);
	}

	if (nullptr != m_hpBarFont)
	{
		wchar_t num[255];
		swprintf_s(num, L"%4d / %4d", (int)curHP, (int)maxHP);
		wstring text = num;

		((CFontUIScript*)m_hpBarFont->GetScripts()[0])->SetText(text);
	}

	if (nullptr != m_spBarFont)
	{
		wchar_t num[255];
		swprintf_s(num, L"%4d / %4d", (int)curSP, (int)maxSP);
		wstring text = num;

		((CFontUIScript*)m_spBarFont->GetScripts()[0])->SetText(text);
	}
}

void CScriptingManager::TrackingBarRatio()
{
	const vector<CGameObject*>& vecTracking = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::UI);

	for (size_t i = 0; i < vecTracking.size(); ++i)
	{
		if (vecTracking[i]->GetScripts().empty())
			continue;

		if (((CUIScript*)vecTracking[i]->GetScripts()[0])->GetTarget() != nullptr)
		{
			CGameObject* target = ((CUIScript*)vecTracking[i]->GetScripts()[0])->GetTarget();
			const vector<CGameObject*>& vecChild = vecTracking[i]->GetChilds();
			for (size_t j = 0; j < vecChild.size(); ++j)
			{
				if (vecChild[j]->GetScripts().empty())
					continue;

				if (((CUIScript*)vecChild[j]->GetScripts()[0])->GetScriptType() == (UINT)SCRIPT_TYPE::BARUISCRIPT)
				{
					float ratio = 1.f;

					if (vecChild[j]->GetName() == L"HPBar")
					{
						ratio = ((CPlayerScript*)target->GetScripts()[0])->GetPlayerHPRatio();
						((CBarUIScript*)vecChild[j]->GetScripts()[0])->SetRatio(ratio);
					}
					else if (vecChild[j]->GetName() == L"MonsterHPBar")
					{
						ratio = ((CMonsterScript*)target->GetScripts()[0])->GetMonsterHPRatio();
						((CBarUIScript*)vecChild[j]->GetScripts()[0])->SetRatio(ratio);
					}
					else if (vecChild[j]->GetName() == L"SPBar")
					{
						if (target->GetObjectType() == OBJECT_TYPE::MONSTER)
						{
							ratio = ((CMonsterScript*)target->GetScripts()[0])->GetMonsterSPRatio();
						}
						else if (target->GetObjectType() == OBJECT_TYPE::PLAYER)
						{
							ratio = ((CPlayerScript*)target->GetScripts()[0])->GetPlayerSPRatio();
						}

						((CBarUIScript*)vecChild[j]->GetScripts()[0])->SetRatio(ratio);
					}
				}
			}
		}
	}
}

void CScriptingManager::PlayerStatFont()
{
	if (nullptr == m_AttackPowerFont)
		m_AttackPowerFont = CSceneManager::GetInst()->FindObjByName(L"AttackPowerFont");
	if (nullptr == m_DefenseFont)
		m_DefenseFont = CSceneManager::GetInst()->FindObjByName(L"DefenseFont");
	if (nullptr == m_ExtraAttackFont)
		m_ExtraAttackFont = CSceneManager::GetInst()->FindObjByName(L"ExtraAttackFont");
	if (nullptr == m_DMGReductionFont)
		m_DMGReductionFont = CSceneManager::GetInst()->FindObjByName(L"DMGReductionFont");
	if (nullptr == m_SkillDMGIncFont)
		m_SkillDMGIncFont = CSceneManager::GetInst()->FindObjByName(L"SkillDMGIncFont");
	if (nullptr == m_SkillDMGReductionFont)
		m_SkillDMGReductionFont = CSceneManager::GetInst()->FindObjByName(L"SkillDMGReductionFont");
	if (nullptr == m_MoveSpeedFont)
		m_MoveSpeedFont = CSceneManager::GetInst()->FindObjByName(L"MoveSpeedFont");
	if (nullptr == m_AttackSpeedFont)
		m_AttackSpeedFont = CSceneManager::GetInst()->FindObjByName(L"AttackSpeedFont");

	CGameObject* owner = nullptr;

	if (nullptr != m_hpBar)
	{
		owner = CSceneManager::GetInst()->FindObjByName(L"Player");
	}

	if (nullptr != owner && nullptr != m_AttackPowerFont)
	{
		float attackPower = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerAttackPower();
		float defense = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerDefense();
		float extraDamage = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerExtraDamage();
		float extraDamagePerLevel = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerExtraDamagePerLevel();
		float damageReduction = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerDamageReduction();
		float skillAmp = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerSkillAmp();
		float skillAmpPerLevel = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerSkillAmpPerLevel();
		float extraSkillDamage = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerExtraSkillDamage();
		float extraSkillDamagePerLevel = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerExtraSkillDamagePerLevel();
		float skillDMGReduction = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerSkillDamageReduction();
		float skillDMGReductionPercent = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerSkillDamageReductionPercent();
		float moveSpeed = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerMoveSpeed();
		float attackSpeed = ((CPlayerScript*)owner->GetScripts()[0])->GetPlayerAttackSpeed();
		float level = (float)((CObjectScript*)owner->GetScripts()[0])->GetLevel();

		((CFontUIScript*)m_AttackPowerFont->GetScripts()[0])->SetNumberText((int)attackPower);
		((CFontUIScript*)m_DefenseFont->GetScripts()[0])->SetNumberText((int)defense);
		((CFontUIScript*)m_ExtraAttackFont->GetScripts()[0])->SetNumberText((int)(extraDamage + (extraDamagePerLevel * level)));

		wchar_t num[255];
		swprintf_s(num, L"%d | 0%%", (int)damageReduction);
		wstring text = num;
		((CFontUIScript*)m_DMGReductionFont->GetScripts()[0])->SetText(text);

		swprintf_s(num, L"%d | %d%%", (int)(extraSkillDamage + extraSkillDamagePerLevel * level), int((skillAmp + skillAmpPerLevel * level) * 100.f));
		text = num;
		((CFontUIScript*)m_SkillDMGIncFont->GetScripts()[0])->SetText(text);

		swprintf_s(num, L"%d | %d%%", (int)skillDMGReduction, int(skillDMGReductionPercent));
		text = num;
		((CFontUIScript*)m_SkillDMGReductionFont->GetScripts()[0])->SetText(text);

		((CFontUIScript*)m_MoveSpeedFont->GetScripts()[0])->SetNumberText((int)moveSpeed);
		((CFontUIScript*)m_AttackSpeedFont->GetScripts()[0])->SetNumberText((int)attackSpeed);
	}
}
