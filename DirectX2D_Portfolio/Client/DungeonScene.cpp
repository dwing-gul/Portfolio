#include "pch.h"
#include "DungeonScene.h"
#include "SceneSaveLoad.h"
#include "PartyManager.h"
#include <Engine/CollisionManager.h>

DungeonScene::DungeonScene()
{
	SetName("DungeonScene");
}

DungeonScene::~DungeonScene()
{
}

void DungeonScene::CreateScene()
{
	SceneSaveLoad::LoadScene(L"scene\\dungeon.scene");

	Ptr<Prefab> skeleton_common = ResourceManager::GetInst()->Load<Prefab>(L"SkeletonCommon", L"prefab\\SkeletonCommon.prefab");
	Ptr<Prefab> cultist_brawler = ResourceManager::GetInst()->Load<Prefab>(L"CultistBrawler", L"prefab\\CultistBrawler.prefab");
	Ptr<Prefab> cultist_witch = ResourceManager::GetInst()->Load<Prefab>(L"CultistWitch", L"prefab\\CultistWitch.prefab");
	Ptr<Prefab> madman = ResourceManager::GetInst()->Load<Prefab>(L"Madman", L"prefab\\Madman.prefab");
	Ptr<Prefab> skeleton_arbalist = ResourceManager::GetInst()->Load<Prefab>(L"SkeletonArbalist", L"prefab\\SkeletonArbalist.prefab");
	Ptr<Prefab> skeleton_courtier = ResourceManager::GetInst()->Load<Prefab>(L"SkeletonCourtier", L"prefab\\SkeletonCourtier.prefab");
	Ptr<Prefab> skeleton_defender = ResourceManager::GetInst()->Load<Prefab>(L"SkeletonDefender", L"prefab\\SkeletonDefender.prefab");
	Ptr<Prefab> pew_large = ResourceManager::GetInst()->Load<Prefab>(L"PewLarge", L"prefab\\PewLarge.prefab");
	Ptr<Prefab> pew_medium = ResourceManager::GetInst()->Load<Prefab>(L"PewMedium", L"prefab\\PewMedium.prefab");
	Ptr<Prefab> pew_small = ResourceManager::GetInst()->Load<Prefab>(L"PewSmall", L"prefab\\PewSmall.prefab");

	PartyManager::GetInst()->SetWaitingMonster(madman.Get());
	PartyManager::GetInst()->SetWaitingMonster(pew_large.Get());
	PartyManager::GetInst()->SetWaitingMonster(pew_medium.Get());
	PartyManager::GetInst()->SetWaitingMonster(pew_small.Get());

	PartyManager::GetInst()->SetWaitingMonster(skeleton_arbalist.Get());
	PartyManager::GetInst()->SetWaitingMonster(cultist_witch.Get());
	PartyManager::GetInst()->SetWaitingMonster(cultist_brawler.Get());
	PartyManager::GetInst()->SetWaitingMonster(cultist_brawler.Get());

	PartyManager::GetInst()->SetWaitingMonster(skeleton_arbalist.Get());
	PartyManager::GetInst()->SetWaitingMonster(skeleton_courtier.Get());
	PartyManager::GetInst()->SetWaitingMonster(skeleton_common.Get());
	PartyManager::GetInst()->SetWaitingMonster(skeleton_defender.Get());

	PartyManager::GetInst()->SetWaitingMonster(cultist_witch.Get());
	PartyManager::GetInst()->SetWaitingMonster(skeleton_common.Get());
	PartyManager::GetInst()->SetWaitingMonster(skeleton_common.Get());
	PartyManager::GetInst()->SetWaitingMonster(cultist_brawler.Get());


	CollisionManager::GetInst()->CollisionCheck(0, 1);
}
