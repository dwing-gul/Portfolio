#include "stdafx.h"
#include "EventManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Texture.h"
#include "ImageUI.h"
#include "Tile.h"
#include "AI.h"
#include "Animator.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::update()
{
	for (size_t i = 0; i < m_vectorDeleteObj.size(); ++i)
	{
		delete m_vectorDeleteObj[i];
	}
	m_vectorDeleteObj.clear();

	for (size_t i = 0; i < m_vectorEvent.size(); ++i)
	{
		Execute(m_vectorEvent[i]);
	}
	m_vectorEvent.clear();
}

void EventManager::Execute(const tEvent& _event)
{
	switch (_event.eType)
	{
	case EVENT_TYPE::CREATE_OBJECT :
	{
		Obj* target = (Obj*)_event.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_event.wParam;

		Scene* curScene = SceneManager::GetInst()->GetCurScene();
		curScene->AddObj(target, eType);
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT :
	{
		Obj* target = (Obj*)_event.lParam;
		target->m_bDead = true;
		m_vectorDeleteObj.push_back(target);
	}
		break;
	case EVENT_TYPE::CHANGE_SCENE :
	{
		SCENE_TYPE eType = (SCENE_TYPE)_event.lParam;

		if (nullptr != SceneManager::GetInst()->m_curScene)
		{
			SceneManager::GetInst()->m_curScene->Exit();
		}

		SceneManager::GetInst()->m_curScene = SceneManager::GetInst()->m_arrScene[(UINT)eType];

		SceneManager::GetInst()->m_curScene->Enter();
	}
		break;
	case EVENT_TYPE::SELECT_TILE :
	{
		int imgIdx = (int)_event.lParam;
		Tile::g_imgIdx = imgIdx;
	}
		break;
	case EVENT_TYPE::CHANGE_STATE :
	{
		AI* ai = (AI*)_event.wParam;
		wstring* state = (wstring*)_event.lParam;
		ai->ChangeState(*state);
		delete state;
	}
		break;
	case EVENT_TYPE::CHANGE_ANIM :
	{
		Animator* animator = (Animator*)_event.wParam;
		wstring* name = (wstring*)_event.lParam;
		animator->ChangeAnim(*name);
		delete name;
	}
		break;
	}
}
