#include "pch.h"
#include "CEventManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

CEventManager::CEventManager() :
	m_bEvent(false),
	m_bOccur(false)
{
}

CEventManager::~CEventManager()
{
}

void CEventManager::update()
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
		Excute(m_vectorEvent[i]);
	}
	m_vectorEvent.clear();

	for (size_t i = 0; i < m_vecLaterEvent.size(); ++i)
	{
		Excute(m_vecLaterEvent[i]);
	}
	m_vecLaterEvent.clear();

	if (m_bOccur)
	{
		m_bEvent = true;
		m_bOccur = false;
	}
}

void CEventManager::Excute(const tEvent& _event)
{
	switch (_event.type)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		CGameObject* obj = (CGameObject*)_event.lParam;
		CScene* curScene = CSceneManager::GetInst()->GetCurScene();
		curScene->AddObject(obj, (int)_event.wParam);

		if (CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		{
			obj->start();
		}

		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		CGameObject* deleteObj = (CGameObject*)_event.lParam;
		deleteObj->SetDead();
		m_vectorDeadObj.push_back(deleteObj);
	}
		break;
	case EVENT_TYPE::ADD_CHILD:
	{
		CGameObject* parent = (CGameObject*)_event.lParam;
		CGameObject* child = (CGameObject*)_event.wParam;

		parent->AddChild(child);
		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::ADD_CHILD_BY_NAME :
	{
		wstring parentName = (const wchar_t*)_event.lParam;
		CGameObject* child = (CGameObject*)_event.wParam;

		vector<CGameObject*> vecParent = CSceneManager::GetInst()->FindObjByName_Vector(parentName);

		for (size_t i = 0; i < vecParent.size(); ++i)
		{
			const vector<wstring>& vecChildName = vecParent[i]->GetChildsName();

			for (size_t j = 0; j < vecChildName.size(); ++j)
			{
				if (child != vecParent[i] && !vecParent[i]->IsAncestor(child) && nullptr == child->GetParent() && child->GetName() == vecChildName[j])
				{
					vecParent[i]->AddChild(child);
					m_bEvent = true;
				}
			}
		}
	}
		break;
	case EVENT_TYPE::DISCONNECT_PARENT:
	{
		CGameObject* child = (CGameObject*)_event.lParam;
		child->DisconnectWithParent();

		CScene* curScene = CSceneManager::GetInst()->GetCurScene();
		CLayer* curLayer = curScene->GetLayer(child->m_layerIndex);
		curLayer->RegisterAsParent(child);
		m_bEvent = true;
	}
	break;
	case EVENT_TYPE::CHANGE_SCENE:
	{
		CScene* nextScene = (CScene*)_event.lParam;
		CSceneManager::GetInst()->ChangeScene(nextScene);
		m_bEvent = true;
	}
		break;
	case EVENT_TYPE::CHANGE_STATE:

		break;
	case EVENT_TYPE::CHANGE_SCENE_MODE:
	{
		CSceneManager::GetInst()->SetSceneMode((SCENE_MODE)_event.lParam);
	}
		break;
	default:
		break;
	}
}
