#include "pch.h"
#include "CItemBoxUIScript.h"
#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include "CItemManager.h"
#include "CItemSpawnScript.h"
#include <Engine/CSceneManager.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>
#include <Engine/CCollider2D.h>

UINT CItemBoxUIScript::g_nextItemID = 0;
UINT CItemBoxUIScript::g_nextBoxID = 0;

CItemBoxUIScript::CItemBoxUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::ITEMBOXUISCRIPT),
	m_Item(ITEM::END),
	m_equipType(EQUIP_TYPE::END),
	m_itemPos(Vec3(0.f, 0.f, 0.f)),
	m_bChange(true),
	m_bEquip(false),
	m_bSlot(false),
	m_bBox(false),
	m_SkillPanelUI(nullptr),
	m_InventoryPanelUI(nullptr),
	m_ProfilePanelUI(nullptr),
	m_PlayerStatPanelUI(nullptr),
	m_ItemBoxPanelUI(nullptr),
	m_itemID(0),
	m_boxID(0)
{

}

CItemBoxUIScript::~CItemBoxUIScript()
{
}

void CItemBoxUIScript::start()
{
	CUIScript::start();

	m_itemPos = GetOriginPos();

	if (nullptr != m_target && !m_bSlot)
	{
		Ptr<CMaterial> material = CResourceManager::GetInst()->FindRes<CMaterial>(L"BarMaterial");
		material->Clone();
		MeshRender()->SetMaterial(material);
	}

	MeshRender()->GetCloneMaterial();

	if (m_bEquip)
	{
		CheckEquip();
	}

	if (nullptr == m_target && !m_bEquip && !m_bSlot)
	{
		m_itemID = g_nextItemID++ % 10;
	}
	else if (nullptr != m_target && !m_bEquip && !m_bSlot)
	{
		m_boxID = g_nextBoxID++ % 16;
		m_bBox = true;
	}
}

void CItemBoxUIScript::update()
{
	CUIScript::update();

	if (nullptr == m_target)
	{
		CheckEquip();
		CheckInventory();
	}
	else
	{
		CheckBox();
	}

	// 아이템 정보가 없다면 그냥 빈 박스로, 있다면 드래그 앤 드롭이 가능하게 해야되며 아이템 정보에 따른 텍스쳐를 띄워야됨
	if (!m_bSlot)
	{
		if (ITEM::END != m_Item)
		{
			m_bDrag = true;
		}
		else
		{
			m_bDrag = false;
		}
	}

	// 드래그 앤 드롭을 했을 때 다른 아이템 박스 위에 있는 것이 아니면 그냥 제자리로 돌아오게 끔 해야됨
	if (m_vecBox.empty())
	{
		m_vecBox = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::UI, true);
	}
	
	if(nullptr == m_SkillPanelUI)
		m_SkillPanelUI = CSceneManager::GetInst()->FindObjByName(L"SkillPanelUI");
	if (nullptr == m_InventoryPanelUI)
		m_InventoryPanelUI = CSceneManager::GetInst()->FindObjByName(L"InventoryPanelUI");
	if (nullptr == m_ProfilePanelUI)
		m_ProfilePanelUI = CSceneManager::GetInst()->FindObjByName(L"ProfilePanelUI");
	if (nullptr == m_PlayerStatPanelUI)
		m_PlayerStatPanelUI = CSceneManager::GetInst()->FindObjByName(L"PlayerStatPanelUI");
	if (nullptr == m_ItemBoxPanelUI)
		m_ItemBoxPanelUI = CSceneManager::GetInst()->FindObjByName(L"ItemBoxPanelUI");

	Vec3 pos = Transform()->GetLocalPos();
	tItem& itemInfo = CItemManager::GetInst()->GetItemInfo(m_Item);

	for (size_t i = 0; i < m_vecBox.size(); ++i)
	{
		if (m_vecBox[i]->GetScripts().empty())
			continue;

		// 지금 오브젝트 위치가 다른 박스 ui 있는지 확인하는거 빼먹음
		Vec3 boxPos = m_vecBox[i]->Transform()->GetLocalPos();
		Vec3 boxScaleHalf = m_vecBox[i]->Transform()->GetLocalScale() / 2.f;
		bool bCanChange = false;

		if (Object() != m_vecBox[i] && m_vecBox[i]->IsOn() && m_vecBox[i]->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMBOXUISCRIPT &&
			!((CItemBoxUIScript*)m_vecBox[i]->GetScripts()[0])->m_bSlot && !m_bSlot && KEY_AWAY(KEY::LBUTTON))
		{
			// 다른 box ui 와 정보를 교환
			if (boxPos.x - boxScaleHalf.x < pos.x && pos.x < boxPos.x + boxScaleHalf.x &&
				boxPos.y - boxScaleHalf.y < pos.y && pos.y < boxPos.y + boxScaleHalf.y)
			{
				if (((CItemBoxUIScript*)m_vecBox[i]->GetScripts()[0])->m_bEquip)
				{
					bCanChange = CanEquipCheck(((CItemBoxUIScript*)m_vecBox[i]->GetScripts()[0]), itemInfo);
				}
				else
				{
					bCanChange = true;
				}
			}
		}

		if (bCanChange)
		{
			ITEM temp = m_Item;
			m_Item = ((CItemBoxUIScript*)m_vecBox[i]->GetScripts()[0])->m_Item;
			((CItemBoxUIScript*)m_vecBox[i]->GetScripts()[0])->m_Item = temp;
			m_bChange = true;
		}
	}

	if (!m_bSlot && KEY_HOLD(KEY::P) && Collider2D()->IsObjectHovered() && IsMouseClicked())
	{
		CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
		m_Item = ITEM::END;

		if (m_bEquip)
		{
			player->ThrowEquip((UINT)m_equipType);
		}
		else
		{
			player->ThrowItem(m_itemID);
		}
	}

	if (!m_bSlot && !m_bBox && Collider2D()->IsObjectHovered() && IsMouseClicked())
	{
		CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
		tItem& item = CItemManager::GetInst()->GetItemInfo(m_Item);

		if (ITEM_TYPE::FOOD == item.Type && player->GetCurHP() != player->GetPlayerMaxHP())
		{
			player->SetFood(true, item.HPGen);
			player->ThrowItem(m_itemID);
		}
		else if (ITEM_TYPE::BEVERAGE == item.Type && player->GetCurSP() != player->GetPlayerMaxSP())
		{
			player->SetBeverage(true, item.SPGen);
			player->ThrowItem(m_itemID);
		}
	}

	if(!IsMove())
		Transform()->SetLocalPos(m_itemPos); // 원래 위치로 돌아가기

	if (nullptr != m_SkillPanelUI)
	{
		Vec3 skillUIAreaPos = m_SkillPanelUI->Transform()->GetLocalPos();
		Vec3 skillUIAreaScaleHalf = m_SkillPanelUI->Transform()->GetLocalScale() / 2.f;

		Vec3 profileUIAreaPos = m_InventoryPanelUI->Transform()->GetLocalPos();
		Vec3 profileUIAreaScaleHalf = m_InventoryPanelUI->Transform()->GetLocalScale() / 2.f;

		Vec3 inventoryUIAreaPos = m_ProfilePanelUI->Transform()->GetLocalPos();
		Vec3 inventoryUIAreaScaleHalf = m_ProfilePanelUI->Transform()->GetLocalScale() / 2.f;

		Vec3 statUIAreaPos = m_PlayerStatPanelUI->Transform()->GetLocalPos();
		Vec3 statUIAreaScaleHalf = m_PlayerStatPanelUI->Transform()->GetLocalScale() / 2.f;

		if (skillUIAreaPos.x - skillUIAreaScaleHalf.x > pos.x && pos.x > skillUIAreaPos.x + skillUIAreaScaleHalf.x &&
			skillUIAreaPos.y - skillUIAreaScaleHalf.y > pos.y && pos.y > skillUIAreaPos.y + skillUIAreaScaleHalf.y &&
			profileUIAreaPos.x - profileUIAreaScaleHalf.x > pos.x && pos.x > profileUIAreaPos.x + profileUIAreaScaleHalf.x &&
			profileUIAreaPos.y - profileUIAreaScaleHalf.y > pos.y && pos.y > profileUIAreaPos.y + profileUIAreaScaleHalf.y &&
			inventoryUIAreaPos.x - inventoryUIAreaScaleHalf.x > pos.x && pos.x > inventoryUIAreaPos.x + inventoryUIAreaScaleHalf.x &&
			inventoryUIAreaPos.y - inventoryUIAreaScaleHalf.y > pos.y && pos.y > inventoryUIAreaPos.y + inventoryUIAreaScaleHalf.y &&
			statUIAreaPos.x - statUIAreaScaleHalf.x > pos.x && pos.x > statUIAreaPos.x + statUIAreaScaleHalf.x &&
			statUIAreaPos.y - statUIAreaScaleHalf.y > pos.y && pos.y > statUIAreaPos.y + statUIAreaScaleHalf.y)
		{
			m_Item = ITEM::END;
			m_bChange = true;
		}
	}
	else if (nullptr != m_ItemBoxPanelUI)
	{
		Vec3 AreaPos = m_ItemBoxPanelUI->Transform()->GetLocalPos();
		Vec3 AreaScaleHalf = m_ItemBoxPanelUI->Transform()->GetLocalScale() / 2.f;

		if (AreaPos.x - AreaScaleHalf.x > pos.x && pos.x > AreaPos.x + AreaScaleHalf.x &&
			AreaPos.y - AreaScaleHalf.y > pos.y && pos.y > AreaPos.y + AreaScaleHalf.y)
		{
			m_Item = ITEM::END;
			m_bChange = true;
		}
	}
}

void CItemBoxUIScript::lateUpdate()
{
	CUIScript::lateUpdate();

	if (m_bChange && MeshRender() && !m_bSlot)
	{
		m_bChange = false;

		SetImage();
	}

	if (nullptr != m_owner && nullptr == m_target && !m_bSlot)
	{
		if (Collider2D()->IsObjectHovered() && IsMouseClicked(MOUSE_BUTTON::RIGHT))
		{
			CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
			if (!m_bEquip && ITEM::END != m_Item)
			{
				tItem& item = CItemManager::GetInst()->GetItemInfo(m_Item);

				const vector<CGameObject*>& vecUI = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::UI, true);

				CItemBoxUIScript* equip = nullptr;

				for (size_t i = 0; i < vecUI.size(); ++i)
				{
					if (!vecUI[i]->GetScripts().empty() && vecUI[i]->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMBOXUISCRIPT)
					{
						EQUIP_TYPE type;

						if ((UINT)item.Type < (UINT)ITEM_TYPE::CHEST)
							type = EQUIP_TYPE::WEAPON;
						else
							type = EQUIP_TYPE((UINT)item.Type - ((UINT)ITEM_TYPE::CHEST - 1));

						if (((CItemBoxUIScript*)vecUI[i]->GetScripts()[0])->m_equipType == type)
						{
							equip = ((CItemBoxUIScript*)vecUI[i]->GetScripts()[0]);
						}
					}
				}

				if (CanEquipCheck(equip, item))
				{
					player->ChangeEquipNInventoryItem((UINT)equip->m_equipType, m_itemID);
				}
			}
			else if(m_bEquip && ITEM::END != m_Item)
			{
				tItem& item = CItemManager::GetInst()->GetItemInfo(m_Item);
				player->ReleaseEquip((UINT)item.Type);
			}
		}
	}
	else if (nullptr != m_owner && nullptr != m_target && !m_bSlot)
	{
		if (Collider2D()->IsObjectHovered() && IsMouseClicked(MOUSE_BUTTON::RIGHT))
		{
			CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];
			player->LootItem(m_Item);

			if (m_target->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMSPAWNSCRIPT)
			{
				CItemSpawnScript* box = (CItemSpawnScript*)m_target->GetScripts()[0];
				box->LootEnd(m_boxID);
			}
			else if (m_target->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::PLAYERSCRIPT)
			{
				CPlayerScript* box = (CPlayerScript*)m_target->GetScripts()[0];
				box->LootEnd(m_boxID);
			}
			else if (m_target->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::MONSTERSCRIPT)
			{
				CMonsterScript* box = (CMonsterScript*)m_target->GetScripts()[0];
				box->LootEnd(m_boxID);
			}
		}
	}
}

void CItemBoxUIScript::PlayerChange()
{
	CUIScript::PlayerChange();

	m_bChange = true;
}

bool CItemBoxUIScript::CanEquipCheck(CItemBoxUIScript* _box, tItem& _itemInfo)
{
	if (nullptr == _box)
		return false;

	if (EQUIP_TYPE::WEAPON == _box->m_equipType)
	{
		const vector<CGameObject*>& vecPlayer = CSceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);

		for (size_t i = 0; i < vecPlayer.size(); ++i)
		{
			if (((CPlayerScript*)vecPlayer[i]->GetScripts()[0])->GetPlayerType() == PLAYER_TYPE::PLAYER)
			{
				const vector<ITEM_TYPE>& vecWeapon = ((CPlayerScript*)vecPlayer[i]->GetScripts()[0])->GetPlayerWeaponType();

				for (size_t j = 0; j < vecWeapon.size(); ++j)
				{
					if (_itemInfo.Type == vecWeapon[j])
					{
						return true;
					}
				}
			}
		}
	}
	else if (EQUIP_TYPE::HEAD == _box->m_equipType && _itemInfo.Type == ITEM_TYPE::HEAD)
	{
		return true;
	}
	else if (EQUIP_TYPE::CHEST == _box->m_equipType && _itemInfo.Type == ITEM_TYPE::CHEST)
	{
		return true;
	}
	else if (EQUIP_TYPE::ARM == _box->m_equipType && _itemInfo.Type == ITEM_TYPE::ARM)
	{
		return true;
	}
	else if (EQUIP_TYPE::LEG == _box->m_equipType && _itemInfo.Type == ITEM_TYPE::LEG)
	{
		return true;
	}
	else if (EQUIP_TYPE::ACCESSORY == _box->m_equipType && _itemInfo.Type == ITEM_TYPE::ACCESSORY)
	{
		return true;
	}

	return false;
}

void CItemBoxUIScript::CheckEquip()
{
	if (nullptr != m_owner && !m_bSlot && m_bEquip)
	{
		CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];

		ITEM item = player->GetEquipItem((UINT)m_equipType);

		m_Item = item;

		SetImage();
	}
}

void CItemBoxUIScript::CheckInventory()
{
	if (nullptr == m_owner || m_bSlot || m_bEquip)
		return;

	CPlayerScript* player = (CPlayerScript*)m_owner->GetScripts()[0];

	ITEM item = player->GetInventoryItem(m_itemID);

	m_Item = item;

	SetImage();
}

void CItemBoxUIScript::CheckBox()
{
	if (m_bSlot)
		return;

	ITEM item = ITEM::END;

	if (m_target->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMSPAWNSCRIPT)
	{
		CItemSpawnScript* box = (CItemSpawnScript*)m_target->GetScripts()[0];
		item = box->GetBoxItem(m_boxID);
	}
	else if (m_target->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	{
		CPlayerScript* box = (CPlayerScript*)m_target->GetScripts()[0];
		item = box->GetBoxItem(m_boxID);
	}
	else if (m_target->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	{
		CMonsterScript* box = (CMonsterScript*)m_target->GetScripts()[0];
		item = box->GetBoxItem(m_boxID);
	}

	m_Item = item;
	SetImage();
}

void CItemBoxUIScript::SetImage()
{
	Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();
	tItem& itemInfo = CItemManager::GetInst()->GetItemInfo(m_Item);
	wstring name = itemInfo.ModelName;
	wstring texturePath = itemInfo.TextureName;
	Ptr<CTexture> itemTex = CResourceManager::GetInst()->Load<CTexture>(name, texturePath);
	Ptr<CTexture> itemBox;
	int mode = 4;

	switch (itemInfo.Grade)
	{
	case ITEM_GRADE::GRAY:
		itemBox = CResourceManager::GetInst()->Load<CTexture>(L"Gray", L"texture\\UI\\Item\\Ico_ItemGradebg_01.png");
		break;
	case ITEM_GRADE::GREEN:
		itemBox = CResourceManager::GetInst()->Load<CTexture>(L"Green", L"texture\\UI\\Item\\Ico_ItemGradebg_02.png");
		break;
	case ITEM_GRADE::BLUE:
		itemBox = CResourceManager::GetInst()->Load<CTexture>(L"Blue", L"texture\\UI\\Item\\Ico_ItemGradebg_03.png");
		break;
	case ITEM_GRADE::PURPLE:
		itemBox = CResourceManager::GetInst()->Load<CTexture>(L"Pupple", L"texture\\UI\\Item\\Ico_ItemGradebg_04.png");
		break;
	case ITEM_GRADE::YELLOW:
		itemBox = CResourceManager::GetInst()->Load<CTexture>(L"Yellow", L"texture\\UI\\Item\\Ico_ItemGradebg_05.png");
		break;
	default:
		itemBox = CResourceManager::GetInst()->Load<CTexture>(L"Empty", L"texture\\UI\\BG\\Img_Main_ItemSlot.png");
		break;
	}

	material->SetData(SHADER_PARAM::TEXTURE_0, itemBox.Get());
	material->SetData(SHADER_PARAM::TEXTURE_1, itemTex.Get());
	material->SetData(SHADER_PARAM::INT_0, &mode);
}

void CItemBoxUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
	SaveToFile(&m_Item, _file);
	SaveToFile(&m_bEquip, _file);
	SaveToFile(&m_equipType, _file);
	SaveToFile(&m_bSlot, _file);
}

void CItemBoxUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
	LoadFromFile(&m_Item, _file);
	LoadFromFile(&m_bEquip, _file);
	LoadFromFile(&m_equipType, _file);
	LoadFromFile(&m_bSlot, _file);
}
