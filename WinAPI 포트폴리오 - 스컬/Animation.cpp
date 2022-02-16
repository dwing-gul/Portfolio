#include "stdafx.h"
#include "Animation.h"
#include "Texture.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Scene_Tool.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Animator.h"
#include "Obj.h"
#include "EventManager.h"
#include "UI.h"
#include "Effect.h"
#include "Boss.h"

Animation::Animation() :
	m_name(),
	m_animator(nullptr),
	m_accTime(0.f),
	m_curFrameIdx(0),
	m_bFinish(false),
	m_bPlay(false)
{
}

Animation::~Animation()
{
	
}

void Animation::lateUpdate()
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if (curScene->GetType() != SCENE_TYPE::TOOL || m_bPlay)
	{
		if (m_bFinish)
			return;

		m_accTime += DELTATIME;

		if (m_vectorFrame[m_curFrameIdx].duration < m_accTime)
		{
			m_accTime -= m_vectorFrame[m_curFrameIdx].duration;
			++m_curFrameIdx;

			if (m_curFrameIdx == m_vectorFrame.size())
			{
				m_bFinish = true;
				m_curFrameIdx -= 1;
			}
		}
	}
}

void Animation::render(HDC _dc)
{
	Vector2 pos = m_animator->GetObj()->GetPos();

	if (m_animator->GetObj()->IsForAnimAdjust())
	{
		TransparentBlt(_dc, int(pos.x - (m_vectorFrame[m_curFrameIdx].slice.x / 2.f + m_vectorFrame[m_curFrameIdx].offset.x)),
			int(pos.y - (m_vectorFrame[m_curFrameIdx].slice.y / 2.f + m_vectorFrame[m_curFrameIdx].offset.y)),
			int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), 
			m_vectorFrame[m_curFrameIdx].texture->GetTextureDC(),
			int(m_vectorFrame[m_curFrameIdx].leftTop.x), int(m_vectorFrame[m_curFrameIdx].leftTop.y),
			int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), RGB(255, 0, 255));
	}
	else if(m_animator->GetObj()->IsItem())
	{
		UI* ui = (UI*)m_animator->GetObj();
		Vector2 finalPos = ui->GetFinalPos();

		if (m_animator->GetObj()->GetName() == L"Ogre")
		{
			TransparentBlt(_dc, int(finalPos.x + (m_vectorFrame[m_curFrameIdx].slice.x / 2.f + m_vectorFrame[m_curFrameIdx].offset.x) - 2.f),
				int(finalPos.y + m_vectorFrame[m_curFrameIdx].offset.y),
				int(m_vectorFrame[m_curFrameIdx].slice.x * 1.5f), int(m_vectorFrame[m_curFrameIdx].slice.y * 1.5f),
				m_vectorFrame[m_curFrameIdx].texture->GetTextureDC(),
				int(m_vectorFrame[m_curFrameIdx].leftTop.x), int(m_vectorFrame[m_curFrameIdx].leftTop.y),
				int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), RGB(255, 0, 255));
		}
		else if (m_animator->GetObj()->GetName() == L"BerserkerSkul")
		{
			TransparentBlt(_dc, int(finalPos.x + 2.f),
				int(finalPos.y + m_vectorFrame[m_curFrameIdx].offset.y),
				int(m_vectorFrame[m_curFrameIdx].slice.x * 1.5f), int(m_vectorFrame[m_curFrameIdx].slice.y * 1.5f),
				m_vectorFrame[m_curFrameIdx].texture->GetTextureDC(),
				int(m_vectorFrame[m_curFrameIdx].leftTop.x), int(m_vectorFrame[m_curFrameIdx].leftTop.y),
				int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), RGB(255, 0, 255));
		}
		else
		{
			TransparentBlt(_dc, int(finalPos.x + (m_vectorFrame[m_curFrameIdx].slice.x / 2.f + m_vectorFrame[m_curFrameIdx].offset.x)),
				int(finalPos.y + (m_vectorFrame[m_curFrameIdx].slice.y / 2.f + m_vectorFrame[m_curFrameIdx].offset.y)),
				int(m_vectorFrame[m_curFrameIdx].slice.x * 1.5f), int(m_vectorFrame[m_curFrameIdx].slice.y * 1.5f),
				m_vectorFrame[m_curFrameIdx].texture->GetTextureDC(),
				int(m_vectorFrame[m_curFrameIdx].leftTop.x), int(m_vectorFrame[m_curFrameIdx].leftTop.y),
				int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), RGB(255, 0, 255));
		}
	}
	else if(((Monster*)m_animator->GetObj())->GetMonsterType() == MONSTER_GROUP::BOSS)
	{
		Vector2 finalPos = ((Boss*)m_animator->GetObj())->GetFinalPos();
		Vector2 renderPos = Camera::GetInst()->GetRenderPos(finalPos);

		TransparentBlt(_dc, int(renderPos.x - (m_vectorFrame[m_curFrameIdx].slice.x * 1.8f / 2.f + m_vectorFrame[m_curFrameIdx].offset.x)),
			int(renderPos.y - (m_vectorFrame[m_curFrameIdx].slice.y * 1.8f / 2.f + m_vectorFrame[m_curFrameIdx].offset.y * 1.8f)),
			int(m_vectorFrame[m_curFrameIdx].slice.x * 1.8f), int(m_vectorFrame[m_curFrameIdx].slice.y * 1.8f),
			m_vectorFrame[m_curFrameIdx].texture->GetTextureDC(),
			int(m_vectorFrame[m_curFrameIdx].leftTop.x), int(m_vectorFrame[m_curFrameIdx].leftTop.y),
			int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), RGB(255, 0, 255));
	}
	else
	{
		Vector2 renderPos = Camera::GetInst()->GetRenderPos(pos);

		TransparentBlt(_dc, int(renderPos.x - (m_vectorFrame[m_curFrameIdx].slice.x * 1.8f / 2.f + m_vectorFrame[m_curFrameIdx].offset.x)),
			int(renderPos.y - (m_vectorFrame[m_curFrameIdx].slice.y * 1.8f / 2.f + m_vectorFrame[m_curFrameIdx].offset.y * 1.8f)),
			int(m_vectorFrame[m_curFrameIdx].slice.x * 1.8f), int(m_vectorFrame[m_curFrameIdx].slice.y * 1.8f),
			m_vectorFrame[m_curFrameIdx].texture->GetTextureDC(),
			int(m_vectorFrame[m_curFrameIdx].leftTop.x), int(m_vectorFrame[m_curFrameIdx].leftTop.y),
			int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), RGB(255, 0, 255));
	}
}

void Animation::EffectRender(HDC _dc)
{
	Vector2 pos = m_animator->GetObj()->GetPos();
	Vector2 magnification = ((Effect*)m_animator->GetObj())->GetMagnification();
	Vector2 renderPos = Camera::GetInst()->GetRenderPos(pos);

	TransparentBlt(_dc, int(renderPos.x - (m_vectorFrame[m_curFrameIdx].slice.x * magnification.x / 2.f + m_vectorFrame[m_curFrameIdx].offset.x)),
		int(renderPos.y - (m_vectorFrame[m_curFrameIdx].slice.y * magnification.y / 2.f + m_vectorFrame[m_curFrameIdx].offset.y * magnification.y)),
		int(m_vectorFrame[m_curFrameIdx].slice.x * magnification.x), int(m_vectorFrame[m_curFrameIdx].slice.y * magnification.y),
		m_vectorFrame[m_curFrameIdx].texture->GetTextureDC(),
		int(m_vectorFrame[m_curFrameIdx].leftTop.x), int(m_vectorFrame[m_curFrameIdx].leftTop.y),
		int(m_vectorFrame[m_curFrameIdx].slice.x), int(m_vectorFrame[m_curFrameIdx].slice.y), RGB(255, 0, 255));
}

void Animation::Create(const wstring& _name, Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum, float _duration)
{
	m_name = _name;

	tAnimFrame frame = {};

	frame.texture = _texture;
	frame.leftTop = _leftTop;
	frame.slice = _slice;
	frame.duration = _duration;
	m_vectorFrame.push_back(frame);
}

void Animation::Delete(int _frameNum)
{
	int count = 0;
	vector<tAnimFrame>::iterator iter = m_vectorFrame.begin();
	Obj* image = SceneManager::GetInst()->GetObjectByName(L"AnimTexture", GROUP_TYPE::UI);

	for (; iter != m_vectorFrame.end();)
	{
		if (_frameNum == count)
		{
			if (image->GetTexture() == iter->texture)
				iter->texture = nullptr;
			else
			{
				if (nullptr != iter->texture)
				{
					delete iter->texture;
					iter->texture = nullptr;
				}
			}

			iter = m_vectorFrame.erase(iter);
			break;
		}
		else
		{
			++iter;
			++count;
		}
	}
}

void Animation::SetAnimSpeed(float _ratio)
{
	if (_ratio <= 0.f)
		assert(nullptr);

	for (size_t i = 0; i < m_vectorFrame.size(); ++i)
	{
		m_vectorDuration.push_back(m_vectorFrame[i].duration);
	}

	for (size_t i = 0; i < m_vectorFrame.size(); ++i)
	{
		m_vectorFrame[i].duration = m_vectorDuration[i] / _ratio;
	}
}

void Animation::Resetting(Texture* _texture, Vector2 _leftTop, Vector2 _slice, int _frameNum)
{
	for (int idx = 0; idx < (int)m_vectorFrame.size(); ++idx)
	{
		if (_frameNum == idx)
		{
			m_vectorFrame[idx].texture = _texture;
			m_vectorFrame[idx].leftTop = _leftTop;
			m_vectorFrame[idx].slice = _slice;
		}

	}
}

void Animation::ChangeAnim(const wstring& _name)
{
	tEvent evn = {};
	evn.eType = EVENT_TYPE::CHANGE_ANIM;
	evn.lParam = (DWORD_PTR)new wstring(_name);
	evn.wParam = (DWORD_PTR)m_animator;

	EventManager::GetInst()->AddEvent(evn);
}

void Animation::Save(FILE* _file)
{
	SaveWString(m_name, _file);

	int frameCount = (int)m_vectorFrame.size();
	fwrite(&frameCount, sizeof(int), 1, _file);

	for (size_t i = 0; i < m_vectorFrame.size(); ++i)
	{
		assert(m_vectorFrame[i].texture);
		SaveWString(m_vectorFrame[i].texture->GetKey(), _file);
		SaveWString(m_vectorFrame[i].texture->GetRelativePath(), _file);
		fwrite(&m_vectorFrame[i].leftTop, sizeof(Vector2), 2, _file);
		fwrite(&m_vectorFrame[i].slice, sizeof(Vector2), 2, _file);
		fwrite(&m_vectorFrame[i].offset, sizeof(Vector2), 2, _file);
		fwrite(&m_vectorFrame[i].duration, sizeof(float), 1, _file);
	}
}

void Animation::Load(FILE* _file)
{
	LoadWString(m_name, _file);

	int frameCount = 0;
	fread(&frameCount, sizeof(int), 1, _file);
	

	tAnimFrame frame = {};
	for (int i = 0; i < frameCount; ++i)
	{
		wstring key, relativePath;
		LoadWString(key, _file);
		LoadWString(relativePath, _file);

		fread(&frame.leftTop, sizeof(Vector2), 2, _file);
		fread(&frame.slice, sizeof(Vector2), 2, _file);
		fread(&frame.offset, sizeof(Vector2), 2, _file);
		fread(&frame.duration, sizeof(float), 1, _file);

		frame.texture = ResourceManager::GetInst()->FindTexture(key);
		if (nullptr == frame.texture)
		{
			frame.texture = ResourceManager::GetInst()->LoadTexture(key, relativePath);
		}
		
		m_vectorFrame.push_back(frame);
	}
}
