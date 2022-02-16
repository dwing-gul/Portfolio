#include "pch.h"
#include "TownObjectScript.h"
#include <Engine/Collider2D.h>
#include <Engine/Animator2D.h>
#include <Engine/Transform.h>

TownObjectScript::TownObjectScript() :
	Script((UINT)SCRIPT_TYPE::TOWNOBJECTSCRIPT),
	m_bStoreOpen(false)
{
}

TownObjectScript::~TownObjectScript()
{
}

void TownObjectScript::start()
{
	if (Object()->GetName() == L"Wagon")
	{
		GetAnimator2D()->LoadAnim(L"anim\\wagon\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\wagon\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Progression")
	{
		GetAnimator2D()->LoadAnim(L"anim\\progression\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\progression\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Tent")
	{
		GetAnimator2D()->LoadAnim(L"anim\\tent\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\tent\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Statue")
	{
		GetAnimator2D()->LoadAnim(L"anim\\statue\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\statue\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Sanitarium")
	{
		GetAnimator2D()->LoadAnim(L"anim\\sanitarium\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\sanitarium\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Pub")
	{
		GetAnimator2D()->LoadAnim(L"anim\\pub\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\pub\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Guild")
	{
		GetAnimator2D()->LoadAnim(L"anim\\guild\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\guild\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Graveyard")
	{
		GetAnimator2D()->LoadAnim(L"anim\\graveyard\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\graveyard\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Coach")
	{
		GetAnimator2D()->LoadAnim(L"anim\\coach\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\coach\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Blacksmith")
	{
		GetAnimator2D()->LoadAnim(L"anim\\blacksmith\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\blacksmith\\Hovered.anim");
	}
	else if (Object()->GetName() == L"Abbey")
	{
		GetAnimator2D()->LoadAnim(L"anim\\abbey\\Normal.anim");
		GetAnimator2D()->LoadAnim(L"anim\\abbey\\Hovered.anim");
	}
}

void TownObjectScript::update()
{
	if (GetCollider2D()->IsObjectHovered())
	{
		GetAnimator2D()->Play(L"Hovered");
	}
	else
	{
		GetAnimator2D()->Play(L"Normal");
	}
}

void TownObjectScript::lateUpdate()
{
	if (Object()->GetName() == L"Pub" && GetCollider2D()->IsObjectHovered() && IsMouseClicked())
	{
		m_bStoreOpen = true;

		Ptr<Prefab> store = ResourceManager::GetInst()->Load<Prefab>(L"StoreBackground", L"prefab\\StoreBackground.prefab");
		Vector3 pos = store->GetPrefabObject()->GetTransform()->GetLocalPos();
		pos.z = 10.f;
		Instantiate(store, pos);

		store = ResourceManager::GetInst()->Load<Prefab>(L"StorePanel", L"prefab\\StorePanel.prefab");
		pos = store->GetPrefabObject()->GetTransform()->GetLocalPos();
		pos.z = 8.f;
		Instantiate(store, pos);

		store = ResourceManager::GetInst()->Load<Prefab>(L"StoreCharacter", L"prefab\\StoreCharacter.prefab");
		pos = store->GetPrefabObject()->GetTransform()->GetLocalPos();
		pos.z = 6.f;
		Instantiate(store, pos);

		store = ResourceManager::GetInst()->Load<Prefab>(L"StoreInventory", L"prefab\\StoreInventory.prefab");
		pos = store->GetPrefabObject()->GetTransform()->GetLocalPos();
		pos.z = 6.f;
		Instantiate(store, pos);

		store = ResourceManager::GetInst()->Load<Prefab>(L"StoreCancel", L"prefab\\StoreCancel.prefab");
		pos = store->GetPrefabObject()->GetTransform()->GetLocalPos();
		pos.z = 6.f;
		Instantiate(store, pos);

		store = ResourceManager::GetInst()->Load<Prefab>(L"StoreItem", L"prefab\\StoreItem.prefab");
		pos = store->GetPrefabObject()->GetTransform()->GetLocalPos();
		pos.z = 6.f;
		Instantiate(store, pos);
	}
}
