#include "pch.h"
#include "TownScene.h"
#include "SceneSaveLoad.h"
#include <Engine/CollisionManager.h>

TownScene::TownScene()
{
	SetName("TownScene");
}

TownScene::~TownScene()
{
}

void TownScene::CreateScene()
{
	SceneSaveLoad::LoadScene(L"scene\\town.scene");
	CollisionManager::GetInst()->CollisionCheck(0, 1);
}
