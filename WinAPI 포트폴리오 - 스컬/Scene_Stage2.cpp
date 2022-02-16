#include "stdafx.h"
#include "Scene_Stage2.h"
#include "Player.h"
#include "Monster.h"
#include "Camera.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "PanelUI.h"
#include "HPBar.h"
#include "TextUI.h"
#include "Core.h"
#include "Texture.h"
#include "Animator.h"
#include "AI.h"
#include "Collider.h"

Scene_Stage2::Scene_Stage2() :
	m_bUISetting(false)
{
}

Scene_Stage2::~Scene_Stage2()
{
}

void Scene_Stage2::update()
{
	Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	vector<SKUL> vectorSkul = player->GetSkuls();

	if (2 == vectorSkul.size() && !m_bUISetting)
	{
		UI* hpUI = (UI*)SceneManager::GetInst()->GetObjectByName(L"PlayerHPUI", GROUP_TYPE::UI);

		PanelUI* childUI = new PanelUI;
		childUI->CreateTexture(L"SmallPortrait", L"Texture\\UI\\SmallPortrait.bmp");
		childUI->SetPos(Vector2(24.f, 70.f));
		childUI->SetName(L"SmallPortrait");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"BerserkerSkulPortraitSmall", L"Texture\\UI\\BerserkerSkulPortraitSmall.bmp");
		childUI->SetPos(Vector2(26.f, 72.f));
		childUI->SetName(L"BerserkerSkulPortraitSmall");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);

		m_bUISetting = true;
	}

	Scene::update();
}

void Scene_Stage2::Enter()
{
	LoadMap(L"Data\\Map\\BossStage.map");
	InitTileUpdate();


	Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

	if (nullptr == player)
	{
		player = new Player;
		player->SetPos(Vector2(64.f * 10.f, 64.f * 11.f));
		player->SetName(L"Player");
		player->CreateAnimator();
		player->GetAnimator()->Load(L"Data\\Anim\\Skul_Basic.anim");
		player->SetAnimDir(player->GetDir(), L"Idle", true);
		player->SkulInit();
		player->SetAttackType(ATTACK_TYPE::PHYSICAL);
		player->SetType(GROUP_TYPE::PLAYER);
		AddObj(player, GROUP_TYPE::PLAYER);
	}
	else
	{
		player->SetPos(Vector2(64.f * 10.f, 64.f * 10.5f));
	}

	Obj* boss = SceneManager::GetInst()->GetObjectByName(L"BossBody", GROUP_TYPE::BOSS_BODY);
	if (nullptr == boss)
	{
		LoadMonster(L"Data\\Map\\BossTest.monster");
		boss = SceneManager::GetInst()->GetObjectByName(L"BossBody", GROUP_TYPE::BOSS_BODY);
	}

	PlayBGM(L"Chapter1_BossBGM", L"Sound\\Chapter1_Boss.wav", true, 0.f, 30.f);

	if (nullptr != player)
	{
		PlayerUISetting(player);

		Camera::GetInst()->SetTarget(player);
		Camera::GetInst()->SetStageEnd(63.f, 0.f, 63.f, 0.f);
	}

	Vector2 resolution = Core::GetInst()->GetResolution();
	PanelUI* hpUI = new PanelUI;
	hpUI->CreateTexture(L"Boss", L"Texture\\UI\\BossUITop.bmp");
	UINT width = hpUI->GetTexture()->GetWidth();
	UINT height = hpUI->GetTexture()->GetHeight();
	hpUI->SetPos(Vector2(resolution.x * 1.1f / 3.f, 2.f));
	hpUI->IsBitBlt(false);

	PanelUI* childUI = new PanelUI;
	childUI->CreateTexture(L"BossUIBar", L"Texture\\UI\\BossUIBar.bmp");
	childUI->SetPos(Vector2(-240.f, 36.f));
	childUI->SetName(L"Boss");
	childUI->IsBitBlt(false);
	hpUI->AddChild(childUI);

	HPBar* hpBar = new HPBar;
	hpBar->CreateTexture(L"HPBar", L"Texture\\UI\\HPBar.bmp");
	hpBar->SetPos(Vector2(-194.f, 60.f));
	hpBar->SetName(L"Boss");
	hpBar->SetTarget(boss);
	hpUI->AddChild(hpBar);

	TextUI* name = new TextUI;
	name->SetText(L"위그드라실");
	name->SetPos(Vector2(208.f, 32.f));
	hpUI->AddChild(name);

	AddObj(hpUI, GROUP_TYPE::UI);

	Collider::SetLook(false);

	CollisionManager::GetInst()->CollisionClear();
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER, GROUP_TYPE::TILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER, GROUP_TYPE::PLAYER_PROJECTILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER, GROUP_TYPE::SUBJECT, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::MONSTER, GROUP_TYPE::TILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER_HITBOX, GROUP_TYPE::BOSS_BODY, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::MONSTER_HITBOX, GROUP_TYPE::PLAYER, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::MONSTER_HITBOX, GROUP_TYPE::TILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::MONSTER, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::TILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::MONSTER_PROJECTILE, GROUP_TYPE::TILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::MONSTER_PROJECTILE, GROUP_TYPE::PLAYER, true);

	Camera::GetInst()->AddCamEffect(CAM_EFFECT::FADE_IN, 1.f);
}

void Scene_Stage2::Exit()
{
	if (SceneManager::GetInst()->IsGoPrev())
	{
		Scene* prevScene = SceneManager::GetInst()->GetPrevScene(GetType());
		Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

		prevScene->AddObj(player, GROUP_TYPE::PLAYER);

		for (int i = 0; i < (int)GROUP_TYPE::END; ++i)
		{
			if ((GROUP_TYPE)i == GROUP_TYPE::PLAYER)
			{
				player->SetGround(false);
				player->SetGroundCount(0);
				player->SetAnimDir(player->GetDir(), L"Idle", true);
				player->SetFirst(false);
				continue;
			}

			DeleteObj((GROUP_TYPE)i);
		}

		ArrClear();

		SceneManager::GetInst()->SetGoPrev(false);
	}
	else
	{
		DeleteAll();
	}

	Camera::GetInst()->SetTarget(nullptr);
	Camera::GetInst()->DeleteAllEffect();
	ResourceManager::GetInst()->DeleteAllSound();
}
