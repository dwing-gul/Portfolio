#include "pch.h"
#include "CToolCamScript.h"
#include <Engine/CKeyManager.h>
#include <Engine/CTransform.h>

CToolCamScript::CToolCamScript() :
	CScript(-1),
	m_speed(700.f)
{
}

CToolCamScript::~CToolCamScript()
{
}

void CToolCamScript::update()
{
	if (CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		return;

	Vector3 pos = Transform()->GetLocalPos();
	Vector3 rot = Transform()->GetLocalRot();
	float speed = m_speed;

	if (KEY_HOLD(KEY::LSHIFT))
	{
		speed *= 4.f;
	}

	Vector3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 right = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	if (KEY_HOLD(KEY::W	))
	{
		pos += speed * front * DELTATIME;
	}
	if (KEY_HOLD(KEY::S))
	{
		pos += speed * -front * DELTATIME;
	}
	if (KEY_HOLD(KEY::A))
	{
		pos += speed * -right * DELTATIME;
	}
	if (KEY_HOLD(KEY::D))
	{
		pos += speed * right * DELTATIME;
	}
	if (KEY_HOLD(KEY::RBUTTON))
	{
		Vec2 dir = CKeyManager::GetInst()->GetMouseDir();
		rot.x += dir.y * XM_PI * DELTATIME;
		rot.y += dir.x * XM_PI * DELTATIME;
	}

	Transform()->SetLocalPos(pos);
	Transform()->SetLocalRot(rot);
}
