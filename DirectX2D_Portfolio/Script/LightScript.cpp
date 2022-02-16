#include "pch.h"
#include "LightScript.h"
#include "CameraScript.h"
#include <Engine/SceneManager.h>
#include <Engine/TimeManager.h>
#include <Engine/KeyManager.h>
#include <Engine/Transform.h>
#include <Engine/Light2D.h>
#include <Engine/GameObject.h>

LightScript::LightScript() :
	Script((UINT)SCRIPT_TYPE::LIGHTSCRIPT),
	m_maxIntensity(1000.f),
	m_curIntensity(1000.f),
	m_lightRatio(0.f)
{
}

LightScript::~LightScript()
{
}

void LightScript::start()
{
}

void LightScript::update()
{
	if (KEY_TAP(KEY::N1))
	{
		m_curIntensity = m_maxIntensity;
	}
	else if (KEY_TAP(KEY::N2))
	{
		m_curIntensity = m_maxIntensity * 0.8f;
	}
	else if (KEY_TAP(KEY::N3))
	{
		m_curIntensity = m_maxIntensity * 0.7f;
	}
	else if (KEY_TAP(KEY::N4))
	{
		m_curIntensity = m_maxIntensity * 0.4f;
	}
	else if (KEY_TAP(KEY::N5))
	{
		m_curIntensity = m_maxIntensity * 0.2f;
	}
	else if (KEY_TAP(KEY::N6))
	{
		m_curIntensity = m_maxIntensity * 0.f;
	}

	m_lightRatio = m_curIntensity / m_maxIntensity;

	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");

	Vector3 cameraPos = camera->GetTransform()->GetLocalPos();

	GetTransform()->SetLocalPos(cameraPos);
}

void LightScript::lateUpdate()
{
	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
	if (!((CameraScript*)camera->GetScripts()[0])->IsEnter())
	{
		m_curIntensity -= 5.f * DELTATIME;

		if (m_curIntensity < 0.f)
			m_curIntensity = 0.f;

		if (m_lightRatio <= 0.f)
		{
			GetLight2D()->SetDiffuseColor(Vector3(0.16f, 0.16f, 0.22f));
		}
		else if (m_lightRatio > 0.f && m_lightRatio <= 0.25f)
		{
			GetLight2D()->SetDiffuseColor(Vector3(0.45f, 0.2f, 0.1f));
		}
		else if (m_lightRatio > 0.25f && m_lightRatio <= 0.5f)
		{
			GetLight2D()->SetDiffuseColor(Vector3(0.5f, 0.3f, 0.2f));
		}
		else if (m_lightRatio > 0.5f && m_lightRatio <= 0.75f)
		{
			GetLight2D()->SetDiffuseColor(Vector3(0.7f, 0.6f, 0.5f));
		}
		else if (m_lightRatio > 0.75f && m_lightRatio <= 0.9f)
		{
			GetLight2D()->SetDiffuseColor(Vector3(0.75f, 0.7f, 0.6f));
		}
		else if (m_lightRatio > 0.9f && m_lightRatio <= 1.f)
		{
			GetLight2D()->SetDiffuseColor(Vector3(0.75f, 0.75f, 0.75f));
		}
	}
}
