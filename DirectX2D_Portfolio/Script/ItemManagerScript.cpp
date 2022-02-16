#include "pch.h"
#include "ItemManagerScript.h"
#include <Engine/SceneManager.h>
#include <Engine/KeyManager.h>

int ItemManagerScript::m_arrItemCount[(UINT)ITEM_TYPE::END] = {};

ItemManagerScript::ItemManagerScript() :
	Script((UINT)SCRIPT_TYPE::ITEMMANAGERSCRIPT)
{
}

ItemManagerScript::~ItemManagerScript()
{
}

void ItemManagerScript::start()
{
}

void ItemManagerScript::update()
{
	GameObject* item = SceneManager::GetInst()->FindObjByName(L"Item_Torch");

	if (nullptr != item && !(item->IsDead()))
	{
		const vector<Script*>& vectorScript = item->GetScripts();

		for (size_t i = 0; i < vectorScript.size(); ++i)
		{
			if (vectorScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMSCRIPT)
			{
				if (((ItemScript*)vectorScript[i])->IsUse())
				{
					if (0 < m_arrItemCount[(UINT)((ItemScript*)vectorScript[i])->GetItemType()])
					{
						m_arrItemCount[(UINT)((ItemScript*)vectorScript[i])->GetItemType()] -= 1;
						((ItemScript*)vectorScript[i])->SetUse(false);
					}

					if (0 >= m_arrItemCount[(UINT)((ItemScript*)vectorScript[i])->GetItemType()])
					{
						m_arrItemCount[(UINT)((ItemScript*)vectorScript[i])->GetItemType()] = 0;
						((ItemScript*)vectorScript[i])->SetZero(true);
					}
				}
			}
		}
	}
}

void ItemManagerScript::lateUpdate()
{
	if (KEY_TAP(KEY::F3))
	{
		m_arrItemCount[(UINT)ITEM_TYPE::TORCH] = 20;
	}

	if (m_arrItemCount[(UINT)ITEM_TYPE::TORCH] > 20)
		m_arrItemCount[(UINT)ITEM_TYPE::TORCH] = 20;
}