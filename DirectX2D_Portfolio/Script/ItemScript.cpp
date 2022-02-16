#include "pch.h"
#include "ItemScript.h"
#include "LightScript.h"
#include <Engine/Collider2D.h>
#include <Engine/Transform.h>
#include <Engine/SceneManager.h>

ItemScript::ItemScript() :
	UIScript(SCRIPT_TYPE::ITEMSCRIPT),
	m_itemType(ITEM_TYPE::TORCH),
	m_bUse(false),
	m_bZero(false)
{
}

ItemScript::~ItemScript()
{
}

void ItemScript::start()
{
	UIScript::start();
}

void ItemScript::update()
{
	UIScript::update();
}

void ItemScript::lateUpdate()
{
	UIScript::lateUpdate();

	if (GetCurTargetUI() == SceneManager::GetInst()->FindObjByName(L"Item_Torch"))
	{
		if (GetCollider2D()->IsObjectHovered() && IsMouseClicked(MOUSE_BUTTON::RIGHT) && !m_bZero)
		{
			m_bUse = true;
		}
	}

	if (m_bUse)
	{
		if (m_itemType == ITEM_TYPE::TORCH)
		{
			GameObject* light = SceneManager::GetInst()->FindObjByName(L"Light");
			const vector<Script*>& vectorScript = light->GetScripts();

			for (size_t i = 0; i < vectorScript.size(); ++i)
			{
				if (vectorScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::LIGHTSCRIPT)
				{
					((LightScript*)vectorScript[i])->AddLight(200.f);
				}
			}
		}
	}
}

void ItemScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_itemType, _file);
}

void ItemScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_itemType, _file);
}
