#include "pch.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

EventManager::EventManager() :
	m_bEvent(false),
	m_bOccur(false)
{
}

EventManager::~EventManager()
{
}

void EventManager::update()
{
	m_bEvent = false;

	for (size_t i = 0; i < m_vectorDeadObj.size(); ++i)
	{
		m_vectorDeadObj[i]->DisconnectWithParent();
		delete m_vectorDeadObj[i];
		m_bEvent = true;
	}
	m_vectorDeadObj.clear();

	for (size_t i = 0; i < m_vectorEvent.size(); ++i)
	{
		Execute(m_vectorEvent[i]);
	}
	m_vectorEvent.clear();

	if (m_bOccur)
	{
		m_bEvent = true;
		m_bOccur = false;
	}
}

void EventManager::Execute(const tEvent& _event)
{
	switch (_event.type)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		GameObject* obj = (GameObject*)_event.lparam;
		Scene* curScene = SceneManager::GetInst()->GetCurScene();
		curScene->AddObj(obj, (int)_event.wparam);

		if (SceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		{
			obj->start();
		}

		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::CREATE_OBJECT_BY_SCENE:
	{
		GameObject* obj = (GameObject*)_event.wparam;
		Scene* scene = (Scene*)_event.lparam;
		scene->AddObj(obj, (int)_event.eparam);

		if (SceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		{
			obj->start();
		}

		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		GameObject* deleteObj = (GameObject*)_event.lparam;
		deleteObj->SetDead();
		m_vectorDeadObj.push_back(deleteObj);
	}
		break;
	case EVENT_TYPE::ADD_CHILD:
	{
		GameObject* parent = (GameObject*)_event.lparam;
		GameObject* child = (GameObject*)_event.wparam;

		parent->AddChild(child);

		if (SceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		{
			child->start();
		}

		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::DISCONNECT_PARENT:
	{
		GameObject* child = (GameObject*)_event.lparam;
		child->DisconnectWithParent();

		Scene* curScene = SceneManager::GetInst()->GetCurScene();
		Layer* curLayer = curScene->GetLayer(child->m_layerIndex);
		curLayer->RegisterAsParent(child);
		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::CHANGE_SCENE:
	{
		Scene* nextScene = (Scene*)_event.lparam;
		SceneManager::GetInst()->ChangeScene(nextScene);
		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::CHANGE_STATE:
		break;
	case EVENT_TYPE::CHANGE_SCENE_MODE:
	{
		SceneManager::GetInst()->SetSceneMode((SCENE_MODE)_event.lparam);
	}
		break;
	}
}
