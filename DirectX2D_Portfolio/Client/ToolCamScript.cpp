#include "pch.h"
#include "ToolCamScript.h"
#include <Engine/KeyManager.h>
#include <Engine/TimeManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Transform.h>

ToolCamScript::ToolCamScript() :
	Script(-1),
	m_speed(500.f)
{
}

ToolCamScript::~ToolCamScript()
{
}

void ToolCamScript::update()
{
	if (SceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		return;

	Vector3 pos = GetTransform()->GetLocalPos();
	Vector3 rot = GetTransform()->GetLocalRot();
	float speed = m_speed;

	if (KEY_HOLD(KEY::LSHIFT))
	{
		speed *= 2.f;
	}
	Vector3 front = GetTransform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 right = GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

	if (KEY_HOLD(KEY::W))
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
		Vector2 dir = KeyManager::GetInst()->GetMouseDir();
		rot.x += dir.y * XM_PI * DELTATIME;
		rot.y += dir.x * XM_PI * DELTATIME;
	}

	GetTransform()->SetLocalPos(pos);
	GetTransform()->SetLocalRot(rot);
}