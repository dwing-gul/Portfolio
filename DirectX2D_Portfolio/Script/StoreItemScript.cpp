#include "pch.h"
#include "StoreItemScript.h"
#include "ItemManagerScript.h"
#include "TownObjectScript.h"
#include <Engine/SceneManager.h>
#include <Engine/Collider2D.h>

StoreItemScript::StoreItemScript() :
	Script((UINT)SCRIPT_TYPE::STOREITEMSCRIPT),
	m_type(BUTTON_TYPE::END)
{
}

StoreItemScript::~StoreItemScript()
{
}

void StoreItemScript::start()
{
}

void StoreItemScript::update()
{
	if (m_type == BUTTON_TYPE::BUY)
	{
		GameObject* itemManager = SceneManager::GetInst()->FindObjByName(L"ItemManager");
		const vector<Script*>& vectorScript = itemManager->GetScripts();
		for (size_t i = 0; i < vectorScript.size(); ++i)
		{
			if (vectorScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMMANAGERSCRIPT)
			{
				if (GetCollider2D()->IsObjectHovered() && IsMouseClicked())
				{
					((ItemManagerScript*)vectorScript[i])->AddItem(ITEM_TYPE::TORCH);
				}
			}
		}
	}
	else if (m_type == BUTTON_TYPE::CANCEL)
	{
		if (GetCollider2D()->IsObjectHovered() && IsMouseClicked())
		{
			GameObject* store = SceneManager::GetInst()->FindObjByName(L"StoreBackground");
			if (nullptr != store && !store->IsDead())
			{
				DeleteObject(store);
			}

			store = SceneManager::GetInst()->FindObjByName(L"StoreCharacter");
			if (nullptr != store && !store->IsDead())
			{
				DeleteObject(store);
			}

			store = SceneManager::GetInst()->FindObjByName(L"StoreInventory");
			if (nullptr != store && !store->IsDead())
			{
				DeleteObject(store);
			}

			store = SceneManager::GetInst()->FindObjByName(L"StorePanel");
			if (nullptr != store && !store->IsDead())
			{
				DeleteObject(store);
			}

			store = SceneManager::GetInst()->FindObjByName(L"StoreItem");
			if (nullptr != store && !store->IsDead())
			{
				DeleteObject(store);
			}

			store = Object();
			if (nullptr != store && !store->IsDead())
			{
				DeleteObject(store);
			}

			GameObject* pub = SceneManager::GetInst()->FindObjByName(L"Pub");
			((TownObjectScript*)pub->GetScripts()[0])->SetStoreOpen(false);
		}
	}
}

void StoreItemScript::lateUpdate()
{
}

void StoreItemScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_type, _file);
}

void StoreItemScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_type, _file);
}
