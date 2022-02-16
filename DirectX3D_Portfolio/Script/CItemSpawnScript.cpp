#include "pch.h"
#include "CItemSpawnScript.h"
#include "CItemManager.h"
#include "CPlayerScript.h"
#include "CUIScript.h"
#include "CMouseRay.h"

CGameObject* CItemSpawnScript::g_itemBox = nullptr;

CItemSpawnScript::CItemSpawnScript() :
	CScript((UINT)SCRIPT_TYPE::ITEMSPAWNSCRIPT),
	m_arrBox{},
	m_spawnArea(APPEAR::PLACE_END),
	m_mouse(nullptr),
	m_bAICheck(true)
{
	
}

CItemSpawnScript::~CItemSpawnScript()
{
}

void CItemSpawnScript::start()
{
	m_mouse = CSceneManager::GetInst()->FindObjByName(L"Mouse");

	for (int i = 0; i < 7; ++i)
	{
		m_arrBox[i] = ITEM::END;
	}

	FillBox();

	if (nullptr == g_itemBox)
	{
		Ptr<CPrefab> prefab = CResourceManager::GetInst()->Load<CPrefab>(L"ItemBoxUI", L"prefab\\RootItemUI.prefab");
		g_itemBox = prefab->Instantiate();
		g_itemBox->SetOnOff(false);
		for (size_t i = 0; i < g_itemBox->GetChilds().size(); ++i)
		{
			((CUIScript*)g_itemBox->GetChilds()[i]->GetScripts()[0])->SetTarget(Object());
		}
		CScript::CreateObject(g_itemBox, 30);
	}

	m_bAICheck = true;
}

void CItemSpawnScript::update()
{
	CGameObject* target = nullptr;
	CGameObject* player = nullptr;
	float dist = 10000.f;

	if (IsMouseClicked(MOUSE_BUTTON::RIGHT) && nullptr != m_mouse)
	{
		target = ((CMouseRay*)m_mouse->GetScripts()[0])->MousePicking();
		if (nullptr != target && OBJECT_TYPE::INTERACTION == target->GetObjectType())
		{
			for (size_t i = 0; i < g_itemBox->GetChilds().size(); ++i)
			{
				((CUIScript*)g_itemBox->GetChilds()[i]->GetScripts()[0])->SetTarget(target);
			}
		}
		player = CSceneManager::GetInst()->FindObjByName(L"Player");
	}

	if (nullptr != player && nullptr != target)
	{
		Vec3 targetPos = target->Transform()->GetWorldPos_XZ();
		Vec3 playerPos = player->Transform()->GetWorldPos_XZ();

		dist = Vec3::Distance(targetPos, playerPos);

		if (Object() == target && dist < 400.f)
		{
			// ¾ÆÀÌÅÛ UI ¿ÀÇÂ
			g_itemBox->SetOnOff(true);
		}
	}

	if (nullptr != g_itemBox && g_itemBox->IsOn() && IsMouseClicked() && nullptr == CUIManager::GetInst()->GetUI() && dist > 400.f)
	{
		// ¾ÆÀÌÅÛ UI ´Ý±â
		g_itemBox->SetOnOff(false);
	}
}

void CItemSpawnScript::lateUpdate()
{
	
}

void CItemSpawnScript::AIGetItems(CPlayerScript* _ai, tBelongings(&_arr)[10])
{
	for (int i = 0; i < 10; ++i)
	{
		if (ITEM::END != _arr[i].ID)
			continue;

		for (int j = 0; j < 7; ++j)
		{
			if (ITEM::END == m_arrBox[j])
				continue;

			vector<tStuff>& vecStuff = _ai->GetStuff();

			for (size_t k = 0; k < vecStuff.size(); ++k)
			{
				if (0 == vecStuff[k].count)
					continue;

				if (m_arrBox[j] == vecStuff[k].item)
				{
					_arr[i].ID = m_arrBox[j];
					_arr[i].count = 1;
					vecStuff[k].count -= 1;
					m_arrBox[j] = ITEM::END;
					break;
				}
			}
		}
	}

	bool checkBox = true;

	for (int i = 0; i < 7; ++i)
	{
		if (ITEM::END != m_arrBox[i])
		{
			checkBox = false;
			break;
		}
	}

	if (checkBox)
	{
		m_bAICheck = false;
	}

	_ai->SetAILootTarget(nullptr);
}

ITEM& CItemSpawnScript::GetBoxItem(int _index)
{
	if (_index > 6 || _index < 0)
	{
		ITEM item = ITEM::END;
		return item;
	}

	return m_arrBox[_index];
}

void CItemSpawnScript::LootEnd(int _index)
{
	if (_index > 6 || _index < 0)
	{
		return;
	}

	m_arrBox[_index] = ITEM::END;
}

void CItemSpawnScript::FillBox()
{
	for (int i = 0; i < (UINT)ITEM::END; ++i)
	{
		if (ITEM::END != m_arrBox[6])
			break;

		tItem& item = CItemManager::GetInst()->GetItemInfo((ITEM)i);

		for (size_t j = 0; j < item.Appear.size(); ++j)
		{
			if (m_spawnArea != item.Appear[j] || item.AppearNum == 0)
				continue;

			srand((unsigned int)time(NULL));
			int lotto = rand() % 100;

			if (lotto < 67)
			{
				for (int k = 0; k < 7; ++k)
				{
					if (ITEM::END == m_arrBox[k])
					{
						m_arrBox[k] = (ITEM)i;
						--item.AppearNum;
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	bool empty = false;

	for (int i = 0; i < 7; ++i)
	{
		if (ITEM::END == m_arrBox[i])
		{
			empty = true;
		}
	}

	if (empty)
	{
		for (int i = 0; i < (UINT)ITEM::END; ++i)
		{
			if (ITEM::END != m_arrBox[6])
				break;

			tItem& item = CItemManager::GetInst()->GetItemInfo((ITEM)i);

			for (size_t j = 0; j < item.Appear.size(); ++j)
			{
				if (m_spawnArea != item.Appear[j] || item.AppearNum == 0)
					continue;

				for (int k = 0; k < 7; ++k)
				{
					if (ITEM::END == m_arrBox[k])
					{
						m_arrBox[k] = (ITEM)i;
						--item.AppearNum;
						break;
					}
				}
			}
		}
	}
}

void CItemSpawnScript::OnColliderEnter(CCollider3D* _other)
{
}

void CItemSpawnScript::OnCollider(CCollider3D* _other)
{
}

void CItemSpawnScript::OnColliderExit(CCollider3D* _other)
{
}

void CItemSpawnScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_spawnArea, _file);
}

void CItemSpawnScript::LoadFromScene(FILE * _file)
{
	LoadFromFile(&m_spawnArea, _file);
}
