#include "pch.h"
#include "CLightScript.h"
#include <Engine/CRenderManager.h>
#include <Engine/CSceneManager.h>
#include <Engine/CTransform.h>
#include <Engine/CLight3D.h>

CLightScript::CLightScript() :
	CScript((UINT)SCRIPT_TYPE::LIGHTSCRIPT),
	m_lightDist(1500.f),
	m_ratio(0.f),
	m_bDay(true),
	m_cam(nullptr)
{
}

CLightScript::~CLightScript()
{
}

void CLightScript::start()
{
	m_cam = CSceneManager::GetInst()->FindObjByName(L"MainCamera")->Camera();
	Vec3 rot = Transform()->GetLocalRot();

	rot.y = 0.f;

	Transform()->SetLocalRot(rot);
}

void CLightScript::update()
{
	if (nullptr == m_cam)
		return;

	Vec3 dir = m_cam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 camPos = m_cam->Transform()->GetLocalPos();

	Vec3 pos = camPos + m_lightDist * dir;
	Vec3 lightCamPos = pos + 500.f * -dir;

	Transform()->SetLocalPos(pos);

	if (m_ratio < 0.0001f)
	{
		Light3D()->GetLightCam()->Transform()->SetLocalPos(lightCamPos);
	}
	else if (m_ratio > 0.0001f && m_ratio < 1.f)
	{
		RotAxis();
	}
}

void CLightScript::lateUpdate()
{
	if (m_bDay)
	{
		if (m_ratio > 0.666f)
		{
			Light3D()->SetDiffuseColor(Vec3(0.949f, 0.522f, 0.22f));
			Light3D()->SetAmbientColor(Vec3(0.7f, 0.7f, 0.7f));
		}
		else if (0.333f < m_ratio && m_ratio < 0.666f)
		{
			Light3D()->SetDiffuseColor(Vec3(0.8f, 0.8f, 0.8f));
			Light3D()->SetAmbientColor(Vec3(0.7f, 0.7f, 0.7f));
		}
		else
		{
			Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
			Light3D()->SetAmbientColor(Vec3(0.7f, 0.7f, 0.7f));
		}
	}
	else
	{
		if (m_ratio > 0.666f)
		{
			Light3D()->SetDiffuseColor(Vec3(0.271f, 0.318f, 0.459f));
			Light3D()->SetAmbientColor(Vec3(0.4f, 0.4f, 0.4f));
		}
		else if (0.333f < m_ratio && m_ratio < 0.666f)
		{
			Light3D()->SetDiffuseColor(Vec3(0.282f, 0.169f, 0.22f));
			Light3D()->SetAmbientColor(Vec3(0.4f, 0.4f, 0.4f));
		}
		else
		{
			Light3D()->SetDiffuseColor(Vec3(0.5373f, 0.4667f, 0.6784f));
			Light3D()->SetAmbientColor(Vec3(0.4f, 0.4f, 0.4f));
		}
	}
}

void CLightScript::RotAxis()
{
	Vec3 rot = Transform()->GetLocalRot();

	rot.x = (75.f - 35.f * m_ratio) * (XM_PI / 180.f);
	rot.y = (90.f * m_ratio) * (XM_PI / 180.f);

	Transform()->SetLocalRot(rot);
}

void CLightScript::SaveToScene(FILE* _file)
{
}

void CLightScript::LoadFromScene(FILE* _file)
{
}
