#include "pch.h"
#include "CSwitchUIScript.h"
#include "CPlayerScript.h"
#include "CCameraScript.h"
#include <Engine/CSceneManager.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMaterial.h>

UINT CSwitchUIScript::g_nextSwitchID = 1;

CSwitchUIScript::CSwitchUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::SWITCHUISCRIPT),
	m_switchID(g_nextSwitchID++)
{
}

CSwitchUIScript::~CSwitchUIScript()
{
}

void CSwitchUIScript::start()
{
	CUIScript::start();

	if (m_switchID > 3)
	{
		m_switchID -= 3;
	}

	const vector<CGameObject*>& vecChar = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);

	for (size_t i = 0; i < vecChar.size(); ++i)
	{
		if (i == m_switchID)
		{
			m_target = vecChar[i];
		}
	}

	MeshRender()->GetCloneMaterial();

	m_cam = CSceneManager::GetInst()->FindObjByName(L"MainCamera");
}

void CSwitchUIScript::update()
{
	CUIScript::update();

	if (nullptr != m_target && ((CPlayerScript*)m_target->GetScripts()[0])->GetPlayerType() == PLAYER_TYPE::AI)
	{
		Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
		Ptr<CTexture> texture;

		if (Collider2D()->IsObjectHovered() && IsMouseClicked())
		{
			const map<wstring, CRes*>& mapSound = CResourceManager::GetInst()->GetRes(RESOURCE_TYPE::SOUND);
			for (auto iter = mapSound.begin(); iter != mapSound.end(); ++iter)
			{
				if (((CSound*)iter->second)->IsLoop())
					continue;
				((CSound*)iter->second)->Stop();
			}

			texture = CResourceManager::GetInst()->Load<CTexture>(L"SwitchClicked", L"texture\\UI\\BG\\Img_Ping_RolloverBg_2.png");
			material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());

			CGameObject* player = CSceneManager::GetInst()->FindObjByName(L"Player");

			wstring playerName = player->GetName();
			wstring targetName = m_target->GetName();

			player->SetName(targetName);
			m_target->SetName(playerName);

			((CPlayerScript*)player->GetScripts()[0])->SetPlayerType(PLAYER_TYPE::AI);
			((CPlayerScript*)player->GetScripts()[0])->SetChange(true);
			((CPlayerScript*)m_target->GetScripts()[0])->SetPlayerType(((CPlayerScript*)m_target->GetScripts()[0])->GetPlayerType());

			const vector<CGameObject*>& vecUI = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::UI, 31, true);

			for (size_t i = 0; i < vecUI.size(); ++i)
			{
				if (vecUI[i]->GetScripts().empty())
					continue;

				if (((CUIScript*)vecUI[i]->GetScripts()[0])->GetTarget() == player)
				{
					((CUIScript*)vecUI[i]->GetScripts()[0])->SetTarget(m_target);
				}
				else if (((CUIScript*)vecUI[i]->GetScripts()[0])->GetTarget() == m_target)
				{
					((CUIScript*)vecUI[i]->GetScripts()[0])->SetTarget(player);
				}

				((CUIScript*)vecUI[i]->GetScripts()[0])->PlayerChange();
				((CCameraScript*)m_cam->GetScripts()[0])->Change();
			}

			m_target = player;

			const vector<CGameObject*>& vecItemBoxUI = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::UI, 30, true);

			for (size_t i = 0; i < vecItemBoxUI.size(); ++i)
			{
				if (vecItemBoxUI[i]->GetScripts().empty())
					continue;

				((CUIScript*)vecItemBoxUI[i]->GetScripts()[0])->PlayerChange();
			}

		}
		else if (Collider2D()->IsObjectHovered() && !IsMouseClicked())
		{
			texture = CResourceManager::GetInst()->Load<CTexture>(L"SwitchHovered", L"texture\\UI\\BG\\Img_Ping_RolloverBg.png");
			material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
		}
		else if (!Collider2D()->IsObjectHovered() && !IsMouseClicked())
		{
			texture = CResourceManager::GetInst()->Load<CTexture>(L"SwitchNone", L"texture\\UI\\BG\\Img_Ping_BoxBg.png");
			material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
		}
	}
	else
	{
		Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
		Ptr<CTexture> texture = CResourceManager::GetInst()->Load<CTexture>(L"SwitchDisable", L"texture\\UI\\BG\\Img_Ping_BoxDisabled_2.png");
		material->SetData(SHADER_PARAM::TEXTURE_0, texture.Get());
	}
}

void CSwitchUIScript::lateUpdate()
{
	CUIScript::lateUpdate();
}

void CSwitchUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
}

void CSwitchUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
}
