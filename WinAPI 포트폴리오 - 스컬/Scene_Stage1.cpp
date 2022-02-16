#include "stdafx.h"
#include "Scene_Stage1.h"
#include "Player.h"
#include "Core.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "Sound.h"
#include "Camera.h"
#include "Subject.h"
#include "Collider.h"
#include "Animator.h"
#include "PanelUI.h"
#include "ImageUI.h"
#include "TextUI.h"
#include "HPBar.h"
#include "Texture.h"

Scene_Stage1::Scene_Stage1() :
	m_bUISetting(false)
{
}

Scene_Stage1::~Scene_Stage1()
{
}

void Scene_Stage1::update()
{
	Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	vector<SKUL> vectorSkul = player->GetSkuls();

	if (2 == vectorSkul.size())
	{
		if (!m_bUISetting)
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
	}

	Scene::update();
}

void Scene_Stage1::Enter()
{
	LoadMap(L"Data\\Map\\testMap.map");
	InitTileUpdate();

	Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

	if (nullptr == player)
	{
		player = new Player;
		player->SetPos(Vector2(32.f * 4.f, 64.f * 8.9f));
		player->SetName(L"Player");
		player->CreateAnimator();
		player->GetAnimator()->Load(L"Data\\Anim\\Skul_Basic.anim");
		player->SetAnimDir(player->GetDir(), L"Idle", true);
		player->SetType(GROUP_TYPE::PLAYER);
		player->SkulInit();
		player->SetAttackType(ATTACK_TYPE::PHYSICAL);
		AddObj(player, GROUP_TYPE::PLAYER);
	}
	else
	{
		player->SetPos(Vector2(32.f * 4.f, 64.f * 8.9f));
	}

	LoadMonster(L"Data\\Map\\test.monster");

	PlayBGM(L"Chapter1BGM", L"Sound\\Chapter1.wav", true, 0.f, 30.f);

	Subject* gate = new Subject;
	gate->SetPos(Vector2(64.f * 22.f, 64.f * 15.f));
	gate->SetSubjectType(SUBJECT_TYPE::GATE);
	gate->CreateAnimator();
	gate->SetName(L"Gate");
	gate->GetAnimator()->Load(L"Data\\Anim\\Gate_General.anim");
	AddObj(gate, GROUP_TYPE::SUBJECT);

	Subject* RingOfWind = new Subject;
	RingOfWind->SetPos(Vector2(32.f * 5.f, 64.f * 8.7f));
	RingOfWind->CreateCollider();
	RingOfWind->GetCollider()->SetOffsetPos(Vector2(0.f, -8.f));
	RingOfWind->GetCollider()->SetScale(Vector2(24.f, 16.f));
	RingOfWind->SetSubjectType(SUBJECT_TYPE::ITEM);
	RingOfWind->SetName(L"RingOfWind");
	RingOfWind->CreateAnimator();
	RingOfWind->GetAnimator()->Load(L"Data\\Anim\\RingOfWind.anim");
	RingOfWind->SetRelativePath(L"Data\\Anim\\RingOfWind.anim");
	RingOfWind->SetItemType(ITEM_TYPE::RING_OF_WIND);
	RingOfWind->SetItemGrade(GRADE::NORMAL);
	AddObj(RingOfWind, GROUP_TYPE::SUBJECT);

	Subject* AdmissionNoticeSeat = new Subject;
	AdmissionNoticeSeat->SetPos(Vector2(32.f * 18.f, 64.f * 8.7f));
	AdmissionNoticeSeat->CreateCollider();
	AdmissionNoticeSeat->GetCollider()->SetOffsetPos(Vector2(0.f, -8.f));
	AdmissionNoticeSeat->GetCollider()->SetScale(Vector2(24.f, 16.f));
	AdmissionNoticeSeat->SetSubjectType(SUBJECT_TYPE::ITEM);
	AdmissionNoticeSeat->SetName(L"AdmissionNoticeSeat");
	AdmissionNoticeSeat->CreateAnimator();
	AdmissionNoticeSeat->GetAnimator()->Load(L"Data\\Anim\\AdmissionNoticeSeat.anim");
	AdmissionNoticeSeat->SetRelativePath(L"Data\\Anim\\AdmissionNoticeSeat.anim");
	AdmissionNoticeSeat->SetItemType(ITEM_TYPE::ADMISSION_NOTICE_SEAT);
	AdmissionNoticeSeat->SetItemGrade(GRADE::NORMAL);
	AddObj(AdmissionNoticeSeat, GROUP_TYPE::SUBJECT);

	Subject* essense = new Subject;
	essense->SetPos(Vector2(32.f * 10.f, 64.f * 8.7f));
	essense->CreateCollider();
	essense->GetCollider()->SetOffsetPos(Vector2(0.f, -12.f));
	essense->GetCollider()->SetScale(Vector2(24.f, 24.f));
	essense->SetSubjectType(SUBJECT_TYPE::ESSENCE);
	essense->CreateAnimator();
	essense->SetName(L"Ogre");
	essense->GetAnimator()->Load(L"Data\\Anim\\Ogre.anim");
	essense->SetRelativePath(L"Data\\Anim\\Ogre.anim");
	essense->SetItemGrade(GRADE::RARE);
	AddObj(essense, GROUP_TYPE::SUBJECT);

	Subject* skul = new Subject;
	skul->SetPos(Vector2(32.f * 14.f, 64.f * 8.7f));
	skul->CreateCollider();
	skul->GetCollider()->SetOffsetPos(Vector2(0.f, -12.f));
	skul->GetCollider()->SetScale(Vector2(24.f, 24.f));
	skul->SetSubjectType(SUBJECT_TYPE::SKUL);
	skul->CreateAnimator();
	skul->SetName(L"BerserkerSkul");
	skul->GetAnimator()->Load(L"Data\\Anim\\BerserkerSkul.anim");
	skul->SetRelativePath(L"Data\\Anim\\BerserkerSkul.anim");
	skul->SetSkul(SKUL::BERSERKER);
	skul->SetItemGrade(GRADE::UNIQUE);
	AddObj(skul, GROUP_TYPE::SUBJECT);

	PlayerUISetting(player);

	Camera::GetInst()->SetTarget(player);
	Camera::GetInst()->SetStageEnd(30.f, 0.f, 532.f, 352.f);
	Camera::GetInst()->AddCamEffect(CAM_EFFECT::FADE_IN, 1.f);

	Collider::SetLook(false);

	CollisionManager::GetInst()->CollisionClear();
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER, GROUP_TYPE::TILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER, GROUP_TYPE::PLAYER_PROJECTILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER, GROUP_TYPE::SUBJECT, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::MONSTER, GROUP_TYPE::TILE, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER_HITBOX, GROUP_TYPE::MONSTER, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::MONSTER_HITBOX, GROUP_TYPE::PLAYER, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::MONSTER, true);
	CollisionManager::GetInst()->ColliderCheck(GROUP_TYPE::PLAYER_PROJECTILE, GROUP_TYPE::TILE, true);
}

void Scene_Stage1::Exit()
{
	if (!SceneManager::GetInst()->IsGoPrev())
	{
		Scene* nextScene = SceneManager::GetInst()->GetNextScene(GetType());
		Player* player = (Player*)SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);

		nextScene->AddObj(player, GROUP_TYPE::PLAYER);

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
	}
	else
	{
		DeleteAll();
	}

	Camera::GetInst()->SetTarget(nullptr);
	Camera::GetInst()->DeleteAllEffect();

	ResourceManager::GetInst()->DeleteAllSound();
}
