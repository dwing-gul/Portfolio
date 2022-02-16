#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Scene_Tool.h"
#include "Scene_Stage1.h"
#include "Scene_Stage2.h"
#include "EventManager.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "Obj.h"
#include "UI.h"
#include "Collider.h"

SceneManager::SceneManager() :
	m_arrScene{},
	m_curScene(nullptr),
	m_bGoPrev(false)
{
}

SceneManager::~SceneManager()
{
	for (int i = 0; i < (int)SCENE_TYPE::END; ++i)
	{
		if(nullptr != m_arrScene[i])
			delete m_arrScene[i];
	}
}

void SceneManager::init()
{
	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new Scene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetType(SCENE_TYPE::TOOL);
	m_arrScene[(UINT)SCENE_TYPE::MAIN] = new Scene_Stage1;
	m_arrScene[(UINT)SCENE_TYPE::MAIN]->SetType(SCENE_TYPE::MAIN);
	m_arrScene[(UINT)SCENE_TYPE::STAGE1] = new Scene_Stage2;
	m_arrScene[(UINT)SCENE_TYPE::STAGE1]->SetType(SCENE_TYPE::STAGE1);

	ChangeScene(SCENE_TYPE::TOOL);
}

void SceneManager::progress()
{
	if (nullptr != m_curScene)
	{
		SCENE_TYPE nextSceneType = (SCENE_TYPE)((UINT)m_curScene->GetType() + 1);

		if (KEY_TAP(KEY::F2) && SCENE_TYPE::END != nextSceneType)
		{
			ChangeScene(nextSceneType);
		}

		if (KEY_TAP(KEY::L))
		{
			if (Collider::GetLook())
			{
				Collider::SetLook(false);
			}
			else
			{
				Collider::SetLook(true);
			}
		}

		if (SCENE_TYPE::TOOL != m_curScene->GetType())
		{
			if (KEY_TAP(KEY::F1))
			{
				SCENE_TYPE prevSceneType = (SCENE_TYPE)((UINT)m_curScene->GetType() - 1);
				m_bGoPrev = true;
				ChangeScene(prevSceneType);
			}
		}

		if (SCENE_TYPE::TOOL != m_curScene->GetType())
		{
			if (KEY_TAP(KEY::TAB))
			{
				UI* inventory = (UI*)GetObjectByName(L"Inventory", GROUP_TYPE::UI);

				if (nullptr != inventory)
				{
					if (inventory->IsLook())
					{
						inventory->SetLook(false);
						PlaySoundEffect(L"InventoryClose", L"Sound\\UI_Inventory_Close.wav", false, 0.f, 30.f);
					}
					else
					{
						inventory->SetLook(true);
						PlaySoundEffect(L"InventoryOpen", L"Sound\\UI_Inventory_Open.wav", false, 0.f, 30.f);
					}

					UIManager::GetInst()->MoveFocusedUI(inventory);
						
					TimeManager::GetInst()->Pause();
				}
			}
		}

		m_curScene->update();
		m_curScene->lateUpdate();
	}
}

void SceneManager::render(HDC _dc)
{
	if (nullptr != m_curScene)
	{
		m_curScene->render(_dc);
	}
}

Obj* SceneManager::GetObjectByName(const wstring& _name, GROUP_TYPE _group)
{
	if (_group == GROUP_TYPE::END)
	{
		for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
		{
			vector<Obj*>& vectorObj = m_curScene->GetObjects((GROUP_TYPE)i);

			for (size_t j = 0; j < vectorObj.size(); ++j)
			{
				if (vectorObj[j]->GetName() == _name)
					return vectorObj[j];
			}
		}
	}
	else
	{
		vector<Obj*>& vectorObj = m_curScene->GetObjects(_group);

		for (size_t i = 0; i < vectorObj.size(); ++i)
		{
			if (vectorObj[i]->GetName() == _name)
				return vectorObj[i];
		}
	}
	return nullptr;
}

void SceneManager::DeleteObjectByName(const wstring& _name, GROUP_TYPE _group)
{
	if (_group == GROUP_TYPE::END)
	{
		for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
		{
			vector<Obj*>& vectorObj = m_curScene->GetObjects((GROUP_TYPE)i);
			vector<Obj*>::iterator iter = vectorObj.begin();

			for (; iter != vectorObj.end();)
			{
				if ((*iter)->GetName() == _name)
				{
					DeleteObject((*iter));
					iter = vectorObj.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}
	else
	{
		vector<Obj*>& vectorObj = m_curScene->GetObjects(_group);
		vector<Obj*>::iterator iter = vectorObj.begin();

		for (; iter != vectorObj.end();)
		{
			if ((*iter)->GetName() == _name)
			{
				DeleteObject((*iter));
				iter = vectorObj.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void SceneManager::ChangeScene(SCENE_TYPE _eType)
{
	tEvent sceneEvent = {};

	sceneEvent.eType = EVENT_TYPE::CHANGE_SCENE;
	sceneEvent.lParam = (DWORD_PTR)_eType;

	EventManager::GetInst()->AddEvent(sceneEvent);
}
