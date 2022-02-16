#include "pch.h"
#include "CCameraScript.h"
#include <Engine/CSceneManager.h>

CCameraScript::CCameraScript() :
	CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT),
	m_bChange(false)
{
	SetName(L"CameraScript");
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::start()
{
	CResourceManager::GetInst()->Load<CSound>(L"AreaBGM_Alley", L"sound\\map\\BSER_AreaBGM_Alley.wav").Get()->Stop();
	CResourceManager::GetInst()->Load<CSound>(L"AreaBGM_Alley", L"sound\\map\\BSER_AreaBGM_Alley.wav").Get()->Play(0, 0.1f);
}

void CCameraScript::update()
{
	CGameObject* player = CSceneManager::GetInst()->FindObjByName(L"Player");
	
	if (nullptr != player)
	{
		if (!m_bChange)
		{
			Vec3 dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
			Vec3 playerPos = player->Transform()->GetLocalPos();
			Vec3 pos = playerPos - 1100.f * dir;

			Transform()->SetLocalPos(pos);
		}
		else
		{
			Vec3 playerPos = player->Transform()->GetLocalPos();
			playerPos.y = 0.f;

			Vec3 pos = Transform()->GetLocalPos();
			Vec3 camPos = Vec3(pos.x, 0.f, pos.z);

			Vec3 dir = (playerPos - camPos).Normalize();
			float dist = Vec3::Distance(playerPos, camPos);

			pos += dir * dist * DELTATIME;

			Transform()->SetLocalPos(pos);

			if (dist < 300.f)
			{
				m_bChange = false;
			}
		}
	}
}

void CCameraScript::lateUpdate()
{
}

void CCameraScript::SaveToScene(FILE* _file)
{
}

void CCameraScript::LoadFromScene(FILE * _file)
{
	SetName(L"CameraScript");
}
