#include "stdafx.h"
#include "Scene.h"
#include "Tile.h"
#include "Background.h"
#include "PathManager.h"
#include "SceneManager.h"
#include "Animation.h"
#include "Animator.h"
#include "Core.h"
#include "Camera.h"
#include "FootMan.h"
#include "HeavyInfantry.h"
#include "Boss.h"
#include "Player.h"
#include "PanelUI.h"
#include "TextUI.h"
#include "ItemImageUI.h"
#include "HPBar.h"
#include "Texture.h"
#include "Collider.h"

Scene::Scene() :
	m_row(0),
	m_col(0),
	m_type(SCENE_TYPE::END)
{
}

Scene::~Scene()
{
	DeleteAll();
}

void Scene::update()
{
	update_tile();

	for (int i = 0; i < (int)GROUP_TYPE::END; ++i)
	{
		if ((int)GROUP_TYPE::TILE == i)
			continue;

		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->update();
		}
	}
}

void Scene::lateUpdate()
{
	lateUpdate_tile();

	for (int i = 0; i < (int)GROUP_TYPE::END; ++i)
	{
		if ((int)GROUP_TYPE::TILE == i)
			continue;

		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->lateUpdate();
		}
	}
}

void Scene::render(HDC _dc)
{
	for (int i = 0; i < (int)GROUP_TYPE::END; ++i)
	{
		if ((int)GROUP_TYPE::TILE == i)
		{
			render_tile(_dc);
			continue;
		}

		vector<Obj*>::iterator iter = m_arrObj[i].begin();
		for (; iter != m_arrObj[i].end(); )
		{
			if ((*iter)->IsDead())
			{
				iter = m_arrObj[i].erase(iter);
			}
			else
			{
				(*iter)->render(_dc);
				++iter;
			}
		}
	}
}

void Scene::CreateTile(UINT _col, UINT _row)
{
	Tile* tile = nullptr;

	for (UINT i = 0; i < _col; ++i)
	{
		for (UINT j = 0; j < _row; ++j)
		{
			tile = new Tile;
			tile->SetPos(Vector2(float(j * TILE_SIZE), float(i * TILE_SIZE)));
			AddObj(tile, GROUP_TYPE::TILE);
		}
	}

	m_col = _col;
	m_row = _row;
}

void Scene::update_tile()
{
	vector<Obj*>& vectorTile = GetObjects(GROUP_TYPE::TILE);

	POINT resolution = Core::GetInst()->GetResolution();

	Vector2 LTPos = Camera::GetInst()->GetRealPos(Vector2(0.f, 0.f));
	int LTIdx = ((int)LTPos.y / TILE_SIZE) * m_row + ((int)LTPos.x / TILE_SIZE);

	int maxRow = resolution.x / TILE_SIZE + 1;
	int maxCol = resolution.y / TILE_SIZE + 1;

	if ((int)m_row - ((int)LTPos.x / TILE_SIZE) < maxRow)
	{
		maxRow = (int)m_row - ((int)LTPos.x / TILE_SIZE);
	}

	if ((int)m_col - ((int)LTPos.y / TILE_SIZE) < maxCol)
	{
		maxCol = (int)m_col - ((int)LTPos.y / TILE_SIZE);
	}
	
	for (int col = 0; col < maxCol; ++col)
	{
		for (int row = 0; row < maxRow; ++row)
		{
			int idx = LTIdx + (m_row * col) + row;

			if (idx < 0)
				continue;

			if (SCENE_TYPE::TOOL != m_type && m_vectorTileUpdate[idx])
			{
				vectorTile[idx]->update();
			}
			else if(SCENE_TYPE::TOOL == m_type)
				vectorTile[idx]->update();
		}
	}
}

void Scene::lateUpdate_tile()
{
	vector<Obj*>& vectorTile = GetObjects(GROUP_TYPE::TILE);

	POINT resolution = Core::GetInst()->GetResolution();

	Vector2 LTPos = Camera::GetInst()->GetRealPos(Vector2(0.f, 0.f));
	int LTIdx = ((int)LTPos.y / TILE_SIZE) * m_row + ((int)LTPos.x / TILE_SIZE);

	int maxRow = resolution.x / TILE_SIZE + 1;
	int maxCol = resolution.y / TILE_SIZE + 1;

	if ((int)m_row - ((int)LTPos.x / TILE_SIZE) < maxRow)
	{
		maxRow = (int)m_row - ((int)LTPos.x / TILE_SIZE);
	}

	if ((int)m_col - ((int)LTPos.y / TILE_SIZE) < maxCol)
	{
		maxCol = (int)m_col - ((int)LTPos.y / TILE_SIZE);
	}

	for (int col = 0; col < maxCol; ++col)
	{
		for (int row = 0; row < maxRow; ++row)
		{
			int idx = LTIdx + (m_row * col) + row;

			if (idx < 0)
				continue;

			if (SCENE_TYPE::TOOL != m_type && m_vectorTileUpdate[idx])
				vectorTile[idx]->lateUpdate();
			else if (SCENE_TYPE::TOOL == m_type)
				vectorTile[idx]->lateUpdate();
		}
	}
}

void Scene::render_tile(HDC _dc)
{
	vector<Obj*>& vectorTile = GetObjects(GROUP_TYPE::TILE);

	POINT resolution = Core::GetInst()->GetResolution();

	Vector2 LTPos = Camera::GetInst()->GetRealPos(Vector2(0.f, 0.f));
	int LTIdx = ((int)LTPos.y / TILE_SIZE) * m_row + ((int)LTPos.x / TILE_SIZE);

	int maxRow = resolution.x / TILE_SIZE + 1;
	int maxCol = resolution.y / TILE_SIZE + 1;

	if ((int)m_row - ((int)LTPos.x / TILE_SIZE) < maxRow)
	{
		maxRow = (int)m_row - ((int)LTPos.x / TILE_SIZE);
	}

	if ((int)m_col - ((int)LTPos.y / TILE_SIZE) < maxCol)
	{
		maxCol = (int)m_col - ((int)LTPos.y / TILE_SIZE);
	}


	for (int col = 0; col < maxCol; ++col)
	{
		for (int row = 0; row < maxRow; ++row)
		{
			int idx = LTIdx + (m_row * col) + row;

			if (idx < 0)
				continue;

			vectorTile[idx]->render(_dc);
		}
	}
}

void Scene::PlayerUISetting(Player* _player)
{
	Vector2 resolution = Core::GetInst()->GetResolution();
	vector<SKUL> vectorSkul = _player->GetSkuls();

	PanelUI* hpUI = new PanelUI;
	hpUI->CreateTexture(L"PlayerHPUI", L"Texture\\UI\\PlayerHPUI.bmp");
	UINT width = hpUI->GetTexture()->GetWidth();
	UINT height = hpUI->GetTexture()->GetHeight();
	hpUI->SetPos(Vector2(0.f, resolution.y - height));
	hpUI->SetName(L"PlayerHPUI");
	hpUI->IsBitBlt(false);

	PanelUI* childUI = new PanelUI;
	if (_player->GetCurSkul() == SKUL::BASIC)
	{
		childUI->CreateTexture(L"BasicSkulIcon", L"Texture\\UI\\BasicSkulPortrait.bmp");
		childUI->SetPos(Vector2(24.f, 2.f));
		childUI->SetName(L"BasicSkulIcon");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"SkilBox", L"Texture\\UI\\SkilBox.bmp");
		childUI->SetPos(Vector2(184.f, 26.f));
		childUI->IsBitBlt(true);
		childUI->SetName(L"SkilBox");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"ThrowingSkul", L"Texture\\UI\\ThrowingSkul.bmp");
		childUI->SetPos(Vector2(127.f, 33.f));
		childUI->SetName(L"ThrowingSkul");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"HeadIsBody", L"Texture\\UI\\HeadIsBody.bmp");
		childUI->SetPos(Vector2(188.f, 32.f));
		childUI->SetName(L"HeadIsBody");
		hpUI->AddChild(childUI);
	}
	else if (_player->GetCurSkul() == SKUL::BERSERKER)
	{
		childUI->CreateTexture(L"BerserkerSkulIcon", L"Texture\\UI\\BasicSkulPortrait.bmp");
		childUI->SetPos(Vector2(24.f, 2.f));
		childUI->SetName(L"BasicSkulIcon");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"SkilBox", L"Texture\\UI\\SkilBox.bmp");
		childUI->SetPos(Vector2(184.f, 26.f));
		childUI->IsBitBlt(true);
		childUI->SetName(L"SkilBox");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"BoneCry", L"Texture\\UI\\BoneCry.bmp");
		childUI->SetPos(Vector2(127.f, 33.f));
		childUI->SetName(L"BoneCry");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"MadAxe", L"Texture\\UI\\MadAxe.bmp");
		childUI->SetPos(Vector2(188.f, 32.f));
		childUI->SetName(L"MadAxe");
		hpUI->AddChild(childUI);
	}

	childUI = childUI->Clone();
	childUI->CreateTexture(L"EssenseBox", L"Texture\\UI\\EssenseBox.bmp");
	childUI->SetPos(Vector2(310.f, 60.f));
	childUI->SetName(L"EssenseBox");
	childUI->SetLook(false);
	childUI->IsBitBlt(false);
	hpUI->AddChild(childUI);

	PanelUI* ogre = new PanelUI;
	ogre->CreateTexture(L"Ogre", L"Texture\\UI\\Ogre.bmp");
	ogre->SetPos(Vector2(46.f, 7.f));
	ogre->SetName(L"Ogre");
	ogre->IsBitBlt(false);
	childUI->AddChild(ogre);

	HPBar* hpBar = new HPBar;
	hpBar->CreateTexture(L"HPBar", L"Texture\\UI\\HPBar.bmp");
	hpBar->SetPos(Vector2(97.f, 88.f));
	hpBar->SetName(L"PlayerHPBar");
	hpBar->SetTarget(_player);
	hpUI->AddChild(hpBar);

	wchar_t intToText[10] = L"";

	TextUI* text = new TextUI;
	_itow_s(_player->GetCurHP(), intToText, 10);
	text->SetText(intToText);
	text->IsCurHP(true);
	text->SetPos(Vector2(190.f, 90.f));
	hpUI->AddChild(text);

	Vector2 textPos = text->GetPos();

	text = text->Clone();
	text->SetText(L"/");
	text->IsCurHP(false);
	text->SetPos(Vector2(textPos.x + 20.f, textPos.y));
	hpUI->AddChild(text);

	textPos = text->GetPos();

	text = text->Clone();
	_itow_s(_player->GetMaxHP(), intToText, 10);
	text->SetText(intToText);
	text->IsCurHP(false);
	text->IsMaxHP(true);
	text->SetPos(Vector2(textPos.x + 20.f, textPos.y));
	hpUI->AddChild(text);

	AddObj(hpUI, GROUP_TYPE::UI);

	PanelUI* inventory = new PanelUI;
	inventory->SetPos(Vector2(10.f, 50.f));
	inventory->CreateTexture(L"Inventory", L"Texture\\UI\\Inventory.bmp");
	inventory->IsBitBlt(false);
	inventory->SetLook(false);
	inventory->SetName(L"Inventory");

	if (1 == vectorSkul.size())
	{
		ItemImageUI* basicSkul = new ItemImageUI;
		basicSkul->SetName(L"Skul");
		basicSkul->SetItemType(SUBJECT_TYPE::SKUL);
		basicSkul->SetItemGrade(GRADE::NORMAL);
		basicSkul->CreateAnimator();
		basicSkul->SetItem(true);
		basicSkul->GetAnimator()->Load(L"Data\\Anim\\BasicSkul.anim");
		basicSkul->SetPos(Vector2(329.f, 164.f));
		basicSkul->SetScale(Vector2(52.f, 52.f));
		basicSkul->SetSkulType(SKUL::BASIC);
		inventory->AddChild(basicSkul);

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
		itemInfo->SetSkulType(SKUL::BASIC);
		inventory->AddChild(itemInfo);
			
	}

	text = text->Clone();
	text->SetText(L"스컬");
	text->IsCurHP(false);
	text->IsMaxHP(false);
	text->IsBlack(true);
	text->SetPos(Vector2(395.f, 132.f));
	inventory->AddChild(text);

	text = text->Clone();
	text->SetText(L"정수");
	text->SetPos(Vector2(395.f, 250.f));
	inventory->AddChild(text);

	text = text->Clone();
	text->SetText(L"아이템");
	text->SetPos(Vector2(395.f, 368.f));
	inventory->AddChild(text);

	AddObj(inventory, GROUP_TYPE::UI);
}

void Scene::UIChange(Player* _player)
{
	UI* hpUI = (UI*)SceneManager::GetInst()->GetObjectByName(L"PlayerHPUI", GROUP_TYPE::UI);
	hpUI->DeleteChildUI();

	PanelUI* childUI = new PanelUI;
	if (_player->GetCurSkul() == SKUL::BASIC)
	{
		childUI->CreateTexture(L"BasicSkulIcon", L"Texture\\UI\\BasicSkulPortrait.bmp");
		childUI->SetPos(Vector2(24.f, 2.f));
		childUI->SetName(L"BasicSkulIcon");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"SkilBox", L"Texture\\UI\\SkilBox.bmp");
		childUI->SetPos(Vector2(184.f, 26.f));
		childUI->IsBitBlt(true);
		childUI->SetName(L"SkilBox");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"ThrowingSkul", L"Texture\\UI\\ThrowingSkul.bmp");
		childUI->SetPos(Vector2(127.f, 33.f));
		childUI->SetName(L"ThrowingSkul");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"HeadIsBody", L"Texture\\UI\\HeadIsBody.bmp");
		childUI->SetPos(Vector2(188.f, 32.f));
		childUI->SetName(L"HeadIsBody");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
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
	}
	else if (_player->GetCurSkul() == SKUL::BERSERKER)
	{
		childUI->CreateTexture(L"BerserkerSkulIcon", L"Texture\\UI\\BerserkerSkulPortrait.bmp");
		childUI->SetPos(Vector2(24.f, 2.f));
		childUI->SetName(L"BasicSkulIcon");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"SkilBox", L"Texture\\UI\\SkilBox.bmp");
		childUI->SetPos(Vector2(184.f, 26.f));
		childUI->IsBitBlt(true);
		childUI->SetName(L"SkilBox");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"BoneCry", L"Texture\\UI\\BoneCry.bmp");
		childUI->SetPos(Vector2(127.f, 33.f));
		childUI->SetName(L"BoneCry");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"MadAxe", L"Texture\\UI\\MadAxe.bmp");
		childUI->SetPos(Vector2(188.f, 32.f));
		childUI->SetName(L"MadAxe");
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"SmallPortrait", L"Texture\\UI\\SmallPortrait.bmp");
		childUI->SetPos(Vector2(24.f, 70.f));
		childUI->SetName(L"SmallPortrait");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);

		childUI = childUI->Clone();
		childUI->CreateTexture(L"BasicSkulPortraitSmall", L"Texture\\UI\\BasicSkulPortraitSmall.bmp");
		childUI->SetPos(Vector2(26.f, 72.f));
		childUI->SetName(L"BasicSkulPortraitSmall");
		childUI->IsBitBlt(false);
		hpUI->AddChild(childUI);
	}

	childUI = childUI->Clone();
	childUI->CreateTexture(L"EssenseBox", L"Texture\\UI\\EssenseBox.bmp");
	childUI->SetPos(Vector2(310.f, 60.f));
	childUI->SetName(L"EssenseBox");
	childUI->SetLook(false);
	childUI->IsBitBlt(false);
	hpUI->AddChild(childUI);

	PanelUI* ogre = new PanelUI;
	ogre->CreateTexture(L"Ogre", L"Texture\\UI\\Ogre.bmp");
	ogre->SetPos(Vector2(46.f, 7.f));
	ogre->SetName(L"Ogre");
	ogre->IsBitBlt(false);
	childUI->AddChild(ogre);

	HPBar* hpBar = new HPBar;
	hpBar->CreateTexture(L"HPBar", L"Texture\\UI\\HPBar.bmp");
	hpBar->SetPos(Vector2(97.f, 88.f));
	hpBar->SetName(L"PlayerHPBar");
	hpBar->SetTarget(_player);
	hpUI->AddChild(hpBar);

	wchar_t intToText[10] = L"";

	TextUI* text = new TextUI;
	_itow_s(_player->GetCurHP(), intToText, 10);
	text->SetText(intToText);
	text->IsCurHP(true);
	text->SetPos(Vector2(190.f, 90.f));
	hpUI->AddChild(text);

	Vector2 textPos = text->GetPos();

	text = text->Clone();
	text->SetText(L"/");
	text->IsCurHP(false);
	text->SetPos(Vector2(textPos.x + 20.f, textPos.y));
	hpUI->AddChild(text);

	textPos = text->GetPos();

	text = text->Clone();
	_itow_s(_player->GetMaxHP(), intToText, 10);
	text->SetText(intToText);
	text->IsCurHP(false);
	text->IsMaxHP(true);
	text->SetPos(Vector2(textPos.x + 20.f, textPos.y));
	hpUI->AddChild(text);
}

void Scene::ArrClear()
{
	for (int i = 0; i < (int)GROUP_TYPE::END; ++i)
	{
		m_arrObj[i].clear();
	}
}

void Scene::LoadMap(const wstring& _relativePath)
{
	DeleteObj(GROUP_TYPE::TILE);

	wstring filePath = CONTENT_PATH;
	filePath += _relativePath;

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");

	assert(file);

	int count = 0;
	fread(&count, sizeof(int), 1, file);

	int temp = 0;
	fread(&temp, sizeof(int), 1, file);

	for (int i = 0; i < count; ++i)
	{
		Background* bg = new Background;
		bg->Load(file);
		AddObj(bg, GROUP_TYPE::BACKGROUND);
	}

	int col = 0;
	int row = 0;

	fread(&col, sizeof(int), 1, file);
	fread(&row, sizeof(int), 1, file);

	int tileCount = col * row;

	CreateTile(col, row);
	vector<Obj*>& vectorTile = GetObjects(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vectorTile.size(); ++i)
	{
		((Tile*)vectorTile[i])->Load(file);
	}

	fclose(file);
}

void Scene::LoadMonster(const wstring& _relativePath)
{
	DeleteObj(GROUP_TYPE::MONSTER);

	wstring filePath = CONTENT_PATH;
	filePath += _relativePath;

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");

	assert(file);

	int count = 0;
	fread(&count, sizeof(int), 1, file);

	int temp = count;

	for (int i = 0; i < temp; ++i)
	{
		MONSTER_GROUP monType = MONSTER_GROUP::FOOTMAN;
		fread(&monType, sizeof(MONSTER_GROUP), 2, file);

		Monster* mon;

		if (monType == MONSTER_GROUP::FOOTMAN)
		{
			mon = new FootMan;
			mon->Load(file);
			mon->SetType(GROUP_TYPE::MONSTER);
			mon->SetMonsterType(monType);
			AddObj(mon, GROUP_TYPE::MONSTER);
		}
		else if (monType == MONSTER_GROUP::HEAVYINFANTRY)
		{
			mon = new HeavyInfantry;
			mon->Load(file);
			mon->SetType(GROUP_TYPE::MONSTER);
			mon->SetMonsterType(monType);
			AddObj(mon, GROUP_TYPE::MONSTER);
		}
		else if (monType == MONSTER_GROUP::BOSS_BODY)
		{
			if (SCENE_TYPE::TOOL == m_type)
			{
				mon = new Boss;
				mon->Load(file);
				mon->SetType(GROUP_TYPE::MONSTER);
				mon->SetMonsterType(MONSTER_GROUP::BOSS);
				mon->SetName(L"BossBody");
				mon->CreateTexture(L"BossBody", L"Texture\\Character\\boss_body.bmp");
			}
			else
			{
				mon = new Boss;
				mon->Load(file);
				mon->SetType(GROUP_TYPE::MONSTER);
				mon->SetMonsterType(MONSTER_GROUP::BOSS);
				mon->SetName(L"BossBody");
				mon->CreateAnimator();
				mon->GetAnimator()->Load(L"Data\\Anim\\Boss_Body.anim");
				mon->CreateCollider();
				mon->GetCollider()->SetScale(Vector2(200.f, 200.f));
				mon->GetCollider()->SetOffsetPos(Vector2(20.f, -280.f));

				Boss* headPart = new Boss;
				headPart->SetPos(Vector2(0.f, -250.f));
				headPart->SetMonsterType(MONSTER_GROUP::BOSS);
				headPart->SetType(GROUP_TYPE::MONSTER);
				headPart->CreateAnimator();
				headPart->GetAnimator()->Load(L"Data\\Anim\\Boss_Head.anim");
				headPart->SetName(L"BossHead");
				((Boss*)mon)->AddPart(headPart);

				Boss* jawPart = new Boss;
				jawPart->SetPos(Vector2(20.f, -200.f));
				jawPart->SetMonsterType(MONSTER_GROUP::BOSS);
				jawPart->CreateAnimator();
				jawPart->GetAnimator()->Load(L"Data\\Anim\\Boss_Jaw.anim");
				jawPart->SetName(L"BossJaw");
				((Boss*)mon)->AddPart(jawPart);
			}

			AddObj(mon, GROUP_TYPE::BOSS_BODY);
		}
		else if (monType == MONSTER_GROUP::BOSS_LEFT_HAND)
		{
			if (SCENE_TYPE::TOOL == m_type)
			{
				mon = new Boss;
				mon->Load(file);
				mon->SetType(GROUP_TYPE::MONSTER);
				mon->SetMonsterType(MONSTER_GROUP::BOSS);
				mon->SetName(L"LeftHand");
				mon->CreateTexture(L"BossLeftHand", L"Texture\\Character\\boss_left_hand.bmp");
			}
			else
			{
				Boss* parent = (Boss*)SceneManager::GetInst()->GetObjectByName(L"BossBody", GROUP_TYPE::BOSS_BODY);

				mon = new Boss;
				mon->Load(file);
				mon->SetType(GROUP_TYPE::MONSTER);
				mon->SetMonsterType(MONSTER_GROUP::BOSS);
				mon->CreateAnimator();
				mon->GetAnimator()->Load(L"Data\\Anim\\Boss_Left_Hand.anim");
				mon->SetName(L"LeftHand");
				parent->SetLeftHands((Boss*)mon);
				((Boss*)mon)->SetParentPart(parent);
			}
			AddObj(mon, GROUP_TYPE::MONSTER);
		}
		else if (monType == MONSTER_GROUP::BOSS_RIGHT_HAND)
		{
			if (SCENE_TYPE::TOOL == m_type)
			{
				mon = new Boss;
				mon->Load(file);
				mon->SetType(GROUP_TYPE::MONSTER);
				mon->SetMonsterType(MONSTER_GROUP::BOSS);
				mon->SetName(L"RightHand");
				mon->CreateTexture(L"BossRightHand", L"Texture\\Character\\boss_right_hand.bmp");
			}
			else
			{
				Boss* parent = (Boss*)SceneManager::GetInst()->GetObjectByName(L"BossBody", GROUP_TYPE::BOSS_BODY);

				mon = new Boss;
				mon->Load(file);
				mon->SetType(GROUP_TYPE::MONSTER);
				mon->SetMonsterType(MONSTER_GROUP::BOSS);
				mon->CreateAnimator();
				mon->GetAnimator()->Load(L"Data\\Anim\\Boss_Right_Hand.anim");
				mon->SetName(L"RightHand");
				parent->SetRightHands((Boss*)mon);
				((Boss*)mon)->SetParentPart(parent);
			}
			AddObj(mon, GROUP_TYPE::MONSTER);
		}
	}

	fclose(file);
}

void Scene::AddObj(Obj* _obj, GROUP_TYPE _eType)
{
	m_arrObj[(UINT)_eType].push_back(_obj);
	_obj->SetType(_eType);
}

void Scene::InitTileUpdate()
{
	vector<Obj*> tile = SceneManager::GetInst()->GetCurScene()->GetObjects(GROUP_TYPE::TILE);

	for (size_t i = 0; i < tile.size(); ++i)
	{
		if (nullptr != tile[i]->GetCollider())
			m_vectorTileUpdate.push_back(true);
		else
			m_vectorTileUpdate.push_back(false);
	}
}

void Scene::DeleteObj(GROUP_TYPE _eType)
{
	for (size_t i = 0; i < m_arrObj[(UINT)_eType].size(); ++i)
	{
		if(!m_arrObj[(UINT)_eType][i]->IsDead())
			delete m_arrObj[(UINT)_eType][i];

		if (SCENE_TYPE::TOOL == m_type && GROUP_TYPE::DEFAULT == _eType)
			DeleteOrder();

		if (GROUP_TYPE::TILE == _eType)
		{
			m_row = 0;
			m_col = 0;
		}
	}

	m_arrObj[(UINT)_eType].clear();
}

void Scene::DeleteOrder()
{
	Obj* image = SceneManager::GetInst()->GetObjectByName(L"AnimTexture", GROUP_TYPE::UI);
	Obj* sample = SceneManager::GetInst()->GetObjectByName(L"Sample", GROUP_TYPE::DEFAULT);

	if (nullptr == sample)
	{
		return;
	}

	Animator* anim = sample->GetAnimator();

	if (nullptr == anim->GetCurAnim())
	{
		return;
	}

	vector<tAnimFrame>& vectorFrame = anim->GetCurAnim()->GetAllFrame();
	vector<tAnimFrame>::iterator iter = vectorFrame.begin();

	for (; iter != vectorFrame.end(); ++iter)
	{
		if ((*iter).texture == image->GetTexture())
		{
			image->SetTexture(nullptr);
		}
	}
}

void Scene::DeleteAll()
{
	for (int i = 0; i < (int)GROUP_TYPE::END; ++i)
	{
		DeleteObj((GROUP_TYPE)i);
	}
}