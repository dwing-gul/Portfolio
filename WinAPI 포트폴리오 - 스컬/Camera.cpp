#include "stdafx.h"
#include "Camera.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Texture.h"
#include "Core.h"
#include "Obj.h"

Camera::Camera() :
	m_look(Vector2(0.f, 0.f)),
	m_diff(Vector2(0.f, 0.f)),
	m_prev(Vector2(0.f, 0.f)),
	m_vibe(Vector2(0.f, 0.f)),
	m_stageLeftEnd(0.f),
	m_stageRightEnd(0.f),
	m_stageTopEnd(0.f),
	m_stageBottomEnd(0.f),
	m_target(nullptr),
	m_speed(300.f),
	m_alpha(0),
	m_veil(nullptr),
	m_bVibration(false)
{
	Vector2 resolution = Core::GetInst()->GetResolution();
	resolution.x /= 2;
	resolution.y /= 2;
	
	m_look = resolution;
}

Camera::~Camera()
{
}

void Camera::init()
{
	POINT res = Core::GetInst()->GetResolution();
	m_veil = ResourceManager::GetInst()->CreateTexture(L"CameraVeilTex", res.x, res.y);
}

void Camera::update()
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if(nullptr != curScene && curScene->GetType() == SCENE_TYPE::TOOL)
		MoveCamera();

	if (nullptr == m_target)
	{
		POINT resolution = Core::GetInst()->GetResolution();
		resolution.x /= 2;
		resolution.y /= 2;

		m_diff = m_look - resolution;
	}
	else
	{
		m_diff = m_target->GetPos() - m_look;

		if (m_diff.x < m_stageLeftEnd)
			m_diff.x = m_stageLeftEnd;

		if (m_diff.x > m_stageRightEnd)
			m_diff.x = m_stageRightEnd;

		if (m_diff.y < m_stageTopEnd)
			m_diff.y = m_stageTopEnd;

		if (m_diff.y > m_stageBottomEnd)
			m_diff.y = m_stageBottomEnd;
	}

	UpdateEffect();
}

void Camera::render(HDC _dc)
{
	if (0 == m_alpha)
		return;

	UINT width = m_veil->GetWidth();
	UINT height = m_veil->GetHeight();

	BLENDFUNCTION blend = {};
	blend.AlphaFormat = 0;
	blend.BlendFlags = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = m_alpha;

	AlphaBlend(_dc, 0, 0, width, height, m_veil->GetTextureDC(), 0, 0, width, height, blend);
}

void Camera::MoveCamera()
{
	if (KEY_HOLD(KEY::W))
	{
		m_look.y -= m_speed * DELTATIME;
	}

	if (KEY_HOLD(KEY::S))
	{
		m_look.y += m_speed * DELTATIME;
	}

	if (KEY_HOLD(KEY::A))
	{
		m_look.x -= m_speed * DELTATIME;
	}

	if (KEY_HOLD(KEY::D))
	{
		m_look.x += m_speed * DELTATIME;
	}
}

void Camera::UpdateEffect()
{
	if (m_listEffect.empty())
		return;

	tCamEffect* effect = &m_listEffect.front();
	effect->curTime += DELTATIME;

	if (effect->endTime < effect->curTime)
	{
		m_listEffect.pop_front();

		if (!m_listEffect.empty())
		{
			m_listEffect.front().curTime = effect->endTime - effect->curTime;
			effect = &m_listEffect.front();
		}
		else
		{
			return;
		}
	}

	float ratio = effect->curTime / effect->endTime;

	if (CAM_EFFECT::FADE_IN == effect->effect)
	{
		ratio = 1.f - ratio;
		m_alpha = (BYTE)(255.f * ratio);
	}
	else if (CAM_EFFECT::FADE_OUT == effect->effect)
	{
		m_alpha = (BYTE)(255.f * ratio);
	}
	else if (CAM_EFFECT::VIBRATION == effect->effect)
	{
		if (effect->curTime < effect->endTime)
		{
			if (m_bVibration)
			{
				m_diff.x -= m_vibe.x;
				m_diff.y -= m_vibe.y;
				m_bVibration = false;
			}
			else
			{
				m_diff.x += m_vibe.x;
				m_diff.y += m_vibe.y;
				m_bVibration = true;
			}
		}
	}
}

void Camera::DeleteAllEffect()
{
	m_alpha = 0;
	m_listEffect.clear();
}

void Camera::SetVibration(Vector2 _range, float _time)
{
	list<tCamEffect>::iterator iter = m_listEffect.begin();
	for(; iter != m_listEffect.end(); ++iter)
	{
		if (CAM_EFFECT::VIBRATION == iter->effect)
		{
			iter->endTime = _time;
			m_prev = m_diff;
			m_vibe = _range;
		}
	}
}

void Camera::StopVibration()
{
	list<tCamEffect>::iterator iter = m_listEffect.begin();
	for (; iter != m_listEffect.end(); )
	{
		if (CAM_EFFECT::VIBRATION == iter->effect)
		{
			iter = m_listEffect.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
