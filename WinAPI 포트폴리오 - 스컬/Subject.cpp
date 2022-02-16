#include "stdafx.h"
#include "Subject.h"
#include "Animator.h"
#include "KeyManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Item.h"
#include "Player.h"
#include "Collider.h"
#include "ItemImageUI.h"
#include "PanelUI.h"
#include "Texture.h"

Subject::Subject() :
	m_subjectType(SUBJECT_TYPE::DEFAULT),
	m_itemType(ITEM_TYPE::NONE),
	m_itemGrade(GRADE::NORMAL),
	m_skul(SKUL::NONE),
	m_relativePath(L"")
{
}

Subject::~Subject()
{
}

void Subject::update()
{
	if (SUBJECT_TYPE::GATE == m_subjectType)
	{
		vector<Obj*> monster = SceneManager::GetInst()->GetCurScene()->GetObjects(GROUP_TYPE::MONSTER);

		if (0 == monster.size())
		{
			GetAnimator()->PlayAnim(L"Active", true);
			PlaySoundEffect(L"GateActivate", L"Sound\\Object_GateActivate.wav", false, 0.f, 100.f);

			if (nullptr == GetCollider())
			{
				CreateCollider();
				GetCollider()->SetOffsetPos(Vector2(0.f, -50.f));
				GetCollider()->SetScale(Vector2(100.f, 100.f));
			}
		}
		else
		{
			GetAnimator()->PlayAnim(L"Deactivate", true);
		}
	}
	else if (SUBJECT_TYPE::ITEM == m_subjectType || SUBJECT_TYPE::ESSENCE == m_subjectType || SUBJECT_TYPE::SKUL == m_subjectType)
	{
		GetAnimator()->PlayAnim(L"Idle", true);
	}
}

void Subject::render(HDC _dc)
{
	if (nullptr != GetAnimator())
	{
		GetAnimator()->render(_dc);
	}

	Obj::render(_dc);
}

void Subject::OnColliderEnter(Obj* _other)
{
}

void Subject::OnCollider(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::PLAYER)
	{
		if (SUBJECT_TYPE::GATE == m_subjectType)
		{
			Scene* curScene = SceneManager::GetInst()->GetCurScene();
			SCENE_TYPE nextSceneType = (SCENE_TYPE)((UINT)curScene->GetType() + 1);

			if (KEY_TAP(KEY::F) && SCENE_TYPE::END != nextSceneType)
			{
				SceneManager::GetInst()->ChangeScene(nextSceneType);
			}
		}

		if (SUBJECT_TYPE::ITEM == m_subjectType)
		{
			PanelUI* inventory = (PanelUI*)SceneManager::GetInst()->GetObjectByName(L"Inventory", GROUP_TYPE::UI);
			int itemCount = inventory->GetItemCount();

			if (KEY_TAP(KEY::F) && itemCount < 9)
			{
				Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

				Item* item = new Item;
				item->SetItemType(m_subjectType);
				item->SetItemStyle(m_itemType);
				item->SetOwner(player);
				item->SetItemGrade(m_itemGrade);
				player->AddItem(item);

				int col = itemCount / 3;
				int row = itemCount % 3;

				ItemImageUI* itemUI = new ItemImageUI;
				itemUI->SetName(GetName());
				itemUI->SetItemType(m_subjectType);
				itemUI->SetItemStyle(m_itemType);
				itemUI->SetItemGrade(m_itemGrade);
				itemUI->CreateAnimator();
				itemUI->GetAnimator()->Load(m_relativePath);
				// x·Î 84, y·Î 64
				itemUI->SetPos(Vector2(286.f + (84.f * row), 400.f + (64.f * col)));
				itemUI->SetScale(Vector2(52.f, 52.f));

				itemUI->SetItem(true);
				inventory->AddChild(itemUI);
				inventory->AddItemCount();

				PanelUI* itemInfo = new PanelUI;
				itemInfo->CreateTexture(L"ItemInfo", L"Texture\\UI\\ItemInfo.bmp");
				UINT width = itemInfo->GetTexture()->GetWidth();
				UINT height = itemInfo->GetTexture()->GetHeight();
				itemInfo->SetScale(Vector2(float(width), float(height)));
				itemInfo->SetName(L"ItemInfo");
				itemInfo->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
				itemInfo->SetPos(Vector2(526.f, 330.f));
				itemInfo->SetSubjectType(m_subjectType);
				itemInfo->SetItemType(m_itemType);
				inventory->AddChild(itemInfo);

				DeleteObject(this);
			}
		}

		if (SUBJECT_TYPE::ESSENCE == m_subjectType)
		{
			PanelUI* inventory = (PanelUI*)SceneManager::GetInst()->GetObjectByName(L"Inventory", GROUP_TYPE::UI);
			if (KEY_TAP(KEY::F))
			{
				Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

				Item* essense = new Item;
				essense->SetItemType(m_subjectType);
				essense->SetOwner(player);
				essense->SetItemGrade(m_itemGrade);
				player->AddEssense(essense);
				delete essense;

				ItemImageUI* essenseUI = new ItemImageUI;
				essenseUI->SetName(GetName());
				essenseUI->SetItemType(m_subjectType);
				essenseUI->SetItemStyle(m_itemType);
				essenseUI->SetItemGrade(m_itemGrade);
				essenseUI->CreateAnimator();
				essenseUI->GetAnimator()->Load(m_relativePath);
				essenseUI->SetPos(Vector2(370.f, 281.f));
				essenseUI->SetScale(Vector2(52.f, 52.f));

				essenseUI->SetItem(true);
				inventory->AddChild(essenseUI);

				PanelUI* itemInfo = new PanelUI;
				itemInfo->CreateTexture(L"EssenseInfo", L"Texture\\UI\\EssenseActive.bmp");
				UINT width = itemInfo->GetTexture()->GetWidth();
				UINT height = itemInfo->GetTexture()->GetHeight();
				itemInfo->SetScale(Vector2(float(width), float(height)));
				itemInfo->SetName(L"ItemInfo");
				itemInfo->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
				itemInfo->SetPos(Vector2(580.f, 400.f));
				itemInfo->SetSubjectType(SUBJECT_TYPE::ESSENCE);
				inventory->AddChild(itemInfo);

				DeleteObject(this);
			}
		}

		if (SUBJECT_TYPE::SKUL == m_subjectType)
		{
			PanelUI* inventory = (PanelUI*)SceneManager::GetInst()->GetObjectByName(L"Inventory", GROUP_TYPE::UI);
			Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

			if (KEY_TAP(KEY::F) && player->GetSkulCount() < 2)
			{
				player->AddSkul(m_skul);

				Item* skul = new Item;
				skul->SetItemType(m_subjectType);
				skul->SetOwner(player);
				skul->SetItemGrade(m_itemGrade);
				delete skul;

				ItemImageUI* skulUI = new ItemImageUI;
				skulUI->SetName(GetName());
				skulUI->SetItemType(m_subjectType);
				skulUI->SetItemStyle(m_itemType);
				skulUI->SetItemGrade(m_itemGrade);
				skulUI->CreateAnimator();
				skulUI->GetAnimator()->Load(m_relativePath);
				skulUI->SetPos(Vector2(412.f, 164.f));
				skulUI->SetScale(Vector2(52.f, 52.f));
				skulUI->SetItem(true);
				skulUI->SetSkulType(m_skul);
				inventory->AddChild(skulUI);

				PanelUI* itemInfo = new PanelUI;
				itemInfo->CreateTexture(L"ItemInfo", L"Texture\\UI\\ItemInfo.bmp");
				UINT width = itemInfo->GetTexture()->GetWidth();
				UINT height = itemInfo->GetTexture()->GetHeight();
				itemInfo->SetScale(Vector2(float(width), float(height)));
				itemInfo->SetName(L"SkulInfo");
				itemInfo->SetUIType(UI_TYPE::ITEM_INFO_PANEL);
				itemInfo->SetPos(Vector2(526.f, 330.f));
				itemInfo->IsBitBlt(true);
				itemInfo->SetLook(false);
				itemInfo->SetSubjectType(SUBJECT_TYPE::SKUL);
				itemInfo->SetSkulType(SKUL::BERSERKER);
				inventory->AddChild(itemInfo);

				DeleteObject(this);
			}
		}
	}
}
