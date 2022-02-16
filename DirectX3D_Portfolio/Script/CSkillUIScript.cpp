#include "pch.h"
#include "CSkillUIScript.h"
#include "CPlayerScript.h"
#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>
#include <Engine/CTexture.h>

CSkillUIScript::CSkillUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::SKILLUISCRIPT),
	m_ownerType(CHARACTER_TYPE::END),
	m_skillType(SKILL_TYPE::Q),
	m_skillMaxLevel(5),
	m_skillCurLevel(0),
	m_bUse(false),
	m_barMode(BAR_MODE::UP_TO_DOWN),
	m_ratio(1.f),
	m_bLevelUp(false),
	m_bStep(false)
{
}

CSkillUIScript::~CSkillUIScript()
{
}

void CSkillUIScript::start()
{
	CUIScript::start();

	if (MeshRender())
	{
		MeshRender()->GetCloneMaterial();

		if (UI_TYPE::SKILL_IMAGE == m_uiType)
		{
			MeshRender()->GetSharedMaterial(0)->SetData(SHADER_PARAM::INT_0, &m_barMode);
		}
		else if (UI_TYPE::SKILL_LEVEL == m_uiType)
		{
		}
	}

	ChangeSkillImage();
}

void CSkillUIScript::update()
{
	CUIScript::update();

	if (UI_TYPE::SKILL_IMAGE == m_uiType)
	{
		if (m_bUse)
		{
			m_ratio = ((CPlayerScript*)m_owner->GetScripts()[0])->GetSkillCTRatio(m_skillType);

			if (m_ratio < 0.f)
				m_bUse = false;
		}
		else
		{
			m_ratio = 0.f;
		}
	}
	else if (UI_TYPE::SKILL_LEVEL == m_uiType && !m_bStep)
	{
		if (m_owner == nullptr)
			return;

		const vector<CGameObject*>& vecChild = Object()->GetChilds();
		CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
		int level = player->GetSkillLevel(m_skillType);
		
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			if (((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bStep)
			{
				if (((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bLevelUp)
				{
					--level;
				}
				else if (!((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bLevelUp && level > 0)
				{
					((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bLevelUp = true;
					Ptr<CMaterial> material = vecChild[i]->MeshRender()->GetSharedMaterial();
					Ptr<CTexture> texture = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevel", L"texture\\UI\\Img_Main_Gage_06.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
					--level;
				}

				if (level == 0)
					break;
			}
		}
		
	}
}

void CSkillUIScript::lateUpdate()
{
	CUIScript::lateUpdate();

	if (UI_TYPE::SKILL_IMAGE == m_uiType)
	{
		if (MeshRender() && nullptr != m_owner)
		{
			CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
			bool bUse = player->IsSkillUsed(m_skillType);
			CMaterial* material = MeshRender()->GetSharedMaterial(0).Get();
			material->SetData(SHADER_PARAM::FLOAT_0, &m_ratio);
			material->SetData(SHADER_PARAM::INT_1, &bUse);
		}
	}
	else if (UI_TYPE::SKILL_LEVEL == m_uiType)
	{
	}
}

void CSkillUIScript::PlayerChange()
{
	CUIScript::PlayerChange();
	ChangeSkillImage();

	const vector<CGameObject*>& vecChild = Object()->GetChilds();
	CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
	int level = player->GetSkillLevel(m_skillType);

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		if (((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bStep)
		{
			Ptr<CMaterial> material = vecChild[i]->MeshRender()->GetSharedMaterial();
			Ptr<CTexture> texture = CResourceManager::GetInst()->FindRes<CTexture>(L"BarBGUI");
			material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
		}
	}

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		if (((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bStep)
		{
			if (((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bLevelUp && level > 0)
			{
				((CSkillUIScript*)vecChild[i]->GetScripts()[0])->m_bLevelUp = true;
				Ptr<CMaterial> material = vecChild[i]->MeshRender()->GetSharedMaterial();
				Ptr<CTexture> texture = CResourceManager::GetInst()->Load<CTexture>(L"SkillLevel", L"texture\\UI\\Img_Main_Gage_06.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
				--level;
			}

			if (level == 0)
				break;
		}
	}
}

void CSkillUIScript::ChangeSkillImage()
{
	if (UI_TYPE::SKILL_IMAGE == m_uiType)
	{
		if (nullptr != m_owner)
		{
			m_ownerType = ((CPlayerScript*)m_owner->GetScripts()[0])->GetCharacterType();
		}

		// ��ų �� �̹��� ���
		if (MeshRender())
		{
			CMaterial* material = MeshRender()->GetSharedMaterial(0).Get();
			Ptr<CTexture> disableBox = CResourceManager::GetInst()->Load<CTexture>(L"DisableBox", L"texture\\UI\\Img_Common_BoxBg_disable.png");
			Ptr<CTexture> skillImage;
			material->SetData(SHADER_PARAM::TEXTURE_1, disableBox.Get());

			switch (m_ownerType)
			{
			case CHARACTER_TYPE::AYA:
			{
				switch (m_skillType)
				{
				case SKILL_TYPE::Q:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Aya_Q", L"texture\\UI\\Aya\\SkillIcon_1002200.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::W:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Aya_W", L"texture\\UI\\Aya\\SkillIcon_1002300.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::E:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Aya_E", L"texture\\UI\\Aya\\SkillIcon_1002400.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::R:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Aya_R", L"texture\\UI\\Aya\\SkillIcon_1002500.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::T:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Aya_T", L"texture\\UI\\Aya\\SkillIcon_1002100.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				}
			}
			break;
			case CHARACTER_TYPE::HYUNWOO:
			{
				switch (m_skillType)
				{
				case SKILL_TYPE::Q:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Hyunwoo_Q", L"texture\\UI\\Hyunwoo\\SkillIcon_1007200.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::W:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Hyunwoo_W", L"texture\\UI\\Hyunwoo\\SkillIcon_1007300.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::E:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Hyunwoo_E", L"texture\\UI\\Hyunwoo\\SkillIcon_1007400.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::R:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Hyunwoo_R", L"texture\\UI\\Hyunwoo\\SkillIcon_1007500.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::T:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Hyunwoo_T", L"texture\\UI\\Hyunwoo\\SkillIcon_1007100.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				}
			}
			break;
			case CHARACTER_TYPE::LIDAILIN:
			{
				switch (m_skillType)
				{
				case SKILL_TYPE::Q:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Lidilin_Q", L"texture\\UI\\Lidilin\\SkillIcon_1010200.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::W:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Lidilin_W", L"texture\\UI\\Lidilin\\SkillIcon_1010300.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::E:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Lidilin_E", L"texture\\UI\\Lidilin\\SkillIcon_1010400.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::R:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Lidilin_R", L"texture\\UI\\Lidilin\\SkillIcon_1010500.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				case SKILL_TYPE::T:
					skillImage = CResourceManager::GetInst()->Load<CTexture>(L"Lidilin_T", L"texture\\UI\\Lidilin\\SkillIcon_1010100.png");
					material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
					break;
				}
			}
			break;
			}
		}
	}
	else if (UI_TYPE::WEAPON_SKILL_IMAGE == m_uiType)
	{
		if (nullptr != m_owner)
		{
			m_ownerType = ((CPlayerScript*)m_owner->GetScripts()[0])->GetCharacterType();
		}

		// ��ų �� �̹��� ���
		if (MeshRender())
		{
			CMaterial* material = MeshRender()->GetSharedMaterial(0).Get();
			Ptr<CTexture> disableBox = CResourceManager::GetInst()->Load<CTexture>(L"DisableBox", L"texture\\UI\\Img_Common_BoxBg_disable.png");
			Ptr<CTexture> skillImage;
			float ratio = 1.f;
			material->SetData(SHADER_PARAM::TEXTURE_1, disableBox.Get());
			material->SetData(SHADER_PARAM::FLOAT_0, &ratio);

			switch (m_ownerType)
			{
			case CHARACTER_TYPE::AYA:
				skillImage = CResourceManager::GetInst()->Load<CTexture>(L"PistolSkill", L"texture\\UI\\Aya\\WSkillIcon_051.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
				break;
			case CHARACTER_TYPE::HYUNWOO:
			case CHARACTER_TYPE::LIDAILIN:
				skillImage = CResourceManager::GetInst()->Load<CTexture>(L"GloveSkill", L"texture\\UI\\Lidilin\\WSkillIcon_081.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, skillImage.Get());
				break;
			}
		}
	}
	else if (UI_TYPE::SKILL_LEVEL == m_uiType && !m_bStep)
	{
	switch (m_skillType)
	{
	case SKILL_TYPE::Q:
	{
		m_skillCurLevel = 0;
		m_skillMaxLevel = 5;
	}
	break;
	case SKILL_TYPE::W:
	{
		m_skillCurLevel = 0;
		m_skillMaxLevel = 5;
	}
		break;
	case SKILL_TYPE::E:
	{
		m_skillCurLevel = 0;
		m_skillMaxLevel = 5;
	}
		break;
	case SKILL_TYPE::R:
	{
		m_skillCurLevel = 0;
		m_skillMaxLevel = 3;
	}
		break;
	case SKILL_TYPE::T:
	{
		m_skillCurLevel = 0;
		m_skillMaxLevel = 3;
	}
		break;
	}
	}
}

void CSkillUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
	SaveToFile(&m_skillType, _file);
	SaveToFile(&m_barMode, _file);
	SaveToFile(&m_bStep, _file);
}

void CSkillUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
	LoadFromFile(&m_skillType, _file);
	LoadFromFile(&m_barMode, _file);
	LoadFromFile(&m_bStep, _file);
}

/* ------------------
* ��ų ui - ��ų ����
* -------------------
* ��ų ������ ���� �κ��� ó���� ȸ�� �簢���� �ִ� ui ���ٰ� ��ų�� ������ �°� 
* ������ �簢�� ���� ȸ�� �簢������ ��Ȳ�� �簢������ �ؽ��ĸ� ���� �����ָ� �ǹǷ�
* � ��ų����, ��ų�� �ִ� ����, ��ų�� ���� ������ �˷��ָ� �ȴ�.
* 
* ---------------------
* ��ų ui - ��ų �̹���
* ---------------------
* ��ų �̹����� ǥ���ϴ� ui. 
* � ��ų���� �˾ƾߵǸ� 0�� �ؽ��ķ� �ش� ��ų�� �´� �̹����� ��������
* 1�� �ؽ��ķ� �������� ������ �簢�� �ؽ��İ� �ʿ��ϴ�.
* �� �� �ִٸ� fx������ ���� ��ų�� ������� �� 1�� �ؽ����� ���� ���̸� ���ݾ� ���̴� ������ ��Ÿ�� ǥ��
*/