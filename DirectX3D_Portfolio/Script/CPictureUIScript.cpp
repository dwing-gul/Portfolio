#include "pch.h"
#include "CPictureUIScript.h"
#include "CPlayerScript.h"

UINT CPictureUIScript::g_nextPictureID = 0;

CPictureUIScript::CPictureUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::PICTUREUISCRIPT),
	m_bProfileChange(true),
	m_pictureID(0)
{
}

CPictureUIScript::~CPictureUIScript()
{
}

void CPictureUIScript::start()
{
	CUIScript::start();

	MeshRender()->GetCloneMaterial();

	if (UI_TYPE::PROFILE_CHAR == m_uiType)
	{
		m_pictureID = g_nextPictureID++ % 4;

		const vector<CGameObject*>& vecPlayer = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);

		for (size_t i = 0; i < vecPlayer.size(); ++i)
		{
			if (i == m_pictureID)
			{
				m_target = vecPlayer[i];
			}
		}
	}

	m_bProfileChange = true;
}

void CPictureUIScript::update()
{
	CUIScript::update();

	if (m_bProfileChange && UI_TYPE::DEFAULT != m_uiType)
	{
		m_bProfileChange = false;

		if (nullptr == m_owner)
			return;

		Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
		Ptr<CTexture> texture;

		if (UI_TYPE::PROFILE == m_uiType)
		{
			switch (((CPlayerScript*)m_owner->GetScripts()[0])->GetCharacterType())
			{
			case CHARACTER_TYPE::AYA:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"Aya_Profile", L"texture\\UI\\Aya\\CharProfile_Aya_S000.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			case CHARACTER_TYPE::HYUNWOO:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"Hyunwoo_Profile", L"texture\\UI\\Hyunwoo\\CharProfile_Hyunwoo_S000.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			case CHARACTER_TYPE::LIDAILIN:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"Lidailin_Profile", L"texture\\UI\\Lidilin\\CharProfile_LiDailin_S000.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			}
		}
		else if (UI_TYPE::WEAPON_ICON == m_uiType)
		{
			switch (((CPlayerScript*)m_owner->GetScripts()[0])->GetCharacterType())
			{
			case CHARACTER_TYPE::AYA:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"WeaponIcon_Pistol", L"texture\\UI\\Ico_Ability_Pistol.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			case CHARACTER_TYPE::HYUNWOO:
			case CHARACTER_TYPE::LIDAILIN:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"WeaponIcon_Gloves", L"texture\\UI\\Ico_Ability_Glove.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			}
		}
		else if (UI_TYPE::PROFILE_CHAR == m_uiType && nullptr != m_target)
		{
			switch (((CPlayerScript*)m_target->GetScripts()[0])->GetCharacterType())
			{
			case CHARACTER_TYPE::AYA:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"Aya_Profile", L"texture\\UI\\Aya\\CharProfile_Aya_S000.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			case CHARACTER_TYPE::HYUNWOO:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"Hyunwoo_Profile", L"texture\\UI\\Hyunwoo\\CharProfile_Hyunwoo_S000.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			case CHARACTER_TYPE::LIDAILIN:
			{
				texture = CResourceManager::GetInst()->Load<CTexture>(L"Lidailin_Profile", L"texture\\UI\\Lidilin\\CharProfile_LiDailin_S000.png");
				material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
			}
			break;
			}
		}
		else if (UI_TYPE::PROFILE_CHAR == m_uiType && nullptr == m_target)
		{
			texture = CResourceManager::GetInst()->Load<CTexture>(L"Aya_Profile", L"texture\\UI\\Aya\\CharProfile_Aya_S000.png");
			material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
		}
	}
}

void CPictureUIScript::lateUpdate()
{
	CUIScript::lateUpdate();
}

void CPictureUIScript::PlayerChange()
{
	CUIScript::PlayerChange();

	m_bProfileChange = true;
}

void CPictureUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
}

void CPictureUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
}