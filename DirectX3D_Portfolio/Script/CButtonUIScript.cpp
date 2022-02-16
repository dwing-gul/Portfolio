#include "pch.h"
#include "CButtonUIScript.h"
#include "CPlayerScript.h"
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>

CButtonUIScript::CButtonUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::BUTTONUISCRIPT),
	m_skillType(SKILL_TYPE::Q),
	m_bClicked(false),
	m_bDisable(false),
	m_bDay(true),
	m_bDayChange(false)
{
}

CButtonUIScript::~CButtonUIScript()
{
}

void CButtonUIScript::start()
{
	CUIScript::start();

	if (MeshRender())
	{
		Ptr<CMaterial> material = MeshRender()->GetCloneMaterial();
		Ptr<CTexture> tex;

		if (UI_TYPE::BUTTON_SKILL == m_uiType)
		{
			tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp", L"texture\\UI\\Btn_LevelUp_Basic.png");
		}
		else if (UI_TYPE::BUTTON_PASSIVE_SKILL == m_uiType)
		{
			tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp_Weapon", L"texture\\UI\\Btn_LevelUp_Basic_02.png");
		}
		else if (UI_TYPE::BUTTON_NONE_DAY == m_uiType)
		{
			tex = CResourceManager::GetInst()->Load<CTexture>(L"DaySun", L"texture\\UI\\BG\\Ico_DaySun.png");
		}

		material->SetData(SHADER_PARAM::TEXTURE_0, tex.Get());
	}

	m_bDayChange = true;
}

void CButtonUIScript::update()
{
	CUIScript::update();

	if (nullptr != m_owner && (UI_TYPE::BUTTON_SKILL == m_uiType || UI_TYPE::BUTTON_PASSIVE_SKILL == m_uiType))
	{
		CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
		int skillPoint = player->GetSkillPoint();

		if (skillPoint == 0)
		{
			Object()->SetRenderCheck(false);
		}
		else
		{
			Object()->SetRenderCheck(true);
		}
	}

	if (MeshRender() && m_bDayChange)
	{
		m_bDayChange = false;

		Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
		Ptr<CTexture> tex;

		if (UI_TYPE::BUTTON_NONE_DAY == m_uiType)
		{
			if (m_bDay)
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"DaySun", L"texture\\UI\\BG\\Ico_DaySun.png");
			}
			else
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"NightMoon", L"texture\\UI\\BG\\Ico_NightMoon.png");
			}
		}

		material->SetData(SHADER_PARAM::TEXTURE_0, tex.Get());
	}

	if (Collider2D()->IsObjectHovered() && !m_bDisable && UI_TYPE::BUTTON_NONE_DAY != m_uiType)
	{
		if (MeshRender())
		{
			Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
			Ptr<CTexture> tex;

			if (UI_TYPE::BUTTON_SKILL == m_uiType)
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp_MouseOn", L"texture\\UI\\Btn_LevelUp_MouseOver.png");
			}
			else if (UI_TYPE::BUTTON_PASSIVE_SKILL == m_uiType)
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp_Passive_MouseOn", L"texture\\UI\\Btn_LevelUp_MouseOver_02.png");
			}

			material->SetData(SHADER_PARAM::TEXTURE_0, tex.Get());
		}

		if(IsMouseClicked())
			m_bClicked = true;
	}
	else if (!Collider2D()->IsObjectHovered() && !m_bDisable && UI_TYPE::BUTTON_NONE_DAY != m_uiType)
	{
		if (MeshRender())
		{
			Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
			Ptr<CTexture> tex;

			if (UI_TYPE::BUTTON_SKILL == m_uiType)
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp", L"texture\\UI\\Btn_LevelUp_Basic.png");
			}
			else if (UI_TYPE::BUTTON_PASSIVE_SKILL == m_uiType)
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp_Weapon", L"texture\\UI\\Btn_LevelUp_Basic_02.png");
			}

			material->SetData(SHADER_PARAM::TEXTURE_0, tex.Get());
		}
	}

	if (nullptr == m_owner)
		return;

	if (m_bClicked)
	{
		m_bClicked = false;

		if (UI_TYPE::BUTTON_SKILL == m_uiType || UI_TYPE::BUTTON_PASSIVE_SKILL == m_uiType)
		{
			KEY type = KEY::KEY_END;

			switch (m_skillType)
			{
			case SKILL_TYPE::Q:
				type = KEY::Q;
				break;
			case SKILL_TYPE::W:
				type = KEY::W;
				break;
			case SKILL_TYPE::E:
				type = KEY::E;
				break;
			case SKILL_TYPE::R:
				type = KEY::R;
				break;
			case SKILL_TYPE::T:
				type = KEY::T;
				break;
			}

			((CPlayerScript*)m_owner->GetScripts()[0])->SkillLvUp(type);
		}
	}
}

void CButtonUIScript::lateUpdate()
{
	CUIScript::lateUpdate();

	if (nullptr == m_owner)
		return;

	if (((CPlayerScript*)m_owner->GetScripts()[0])->CanSkillLevelUp(m_skillType))
	{
		m_bDisable = false;
	}
	else
	{
		m_bDisable = true;
	}

	if (m_bDisable && UI_TYPE::BUTTON_NONE_DAY != m_uiType)
	{
		if (MeshRender())
		{
			Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
			Ptr<CTexture> tex;

			if (UI_TYPE::BUTTON_SKILL == m_uiType)
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp_Disable", L"texture\\UI\\Btn_LevelUp_Disabled.png");
			}
			else if (UI_TYPE::BUTTON_PASSIVE_SKILL == m_uiType)
			{
				tex = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevelUp_Passive_Disable", L"texture\\UI\\Btn_LevelUp_Disabled_02.png");
			}
			else if (UI_TYPE::BUTTON == m_uiType)
			{
			}

			material->SetData(SHADER_PARAM::TEXTURE_0, tex.Get());
		}
	}
}

void CButtonUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
	SaveToFile(&m_skillType, _file);
}

void CButtonUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
	LoadFromFile(&m_skillType, _file);
}
