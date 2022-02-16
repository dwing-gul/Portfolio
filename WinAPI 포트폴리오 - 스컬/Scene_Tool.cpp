#include "stdafx.h"
#include "Scene_Tool.h"
#include "Core.h"
#include "PathManager.h"
#include "SceneManager.h"
#include "KeyManager.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "resource.h"
#include "Tile.h"
#include "NPC.h"
#include "Monster.h"
#include "PanelUI.h"
#include "Collider.h"
#include "Texture.h"
#include "HollowBox.h"
#include "Animation.h"
#include "Animator.h"
#include "Background.h"
#include "Collider.h"

Scene_Tool::Scene_Tool() :
	m_menu(nullptr)
{
	m_menu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_SKUL));
}

Scene_Tool::~Scene_Tool()
{
	DestroyWindow(hDlg);
	DestroyMenu(m_menu);
	DeleteOrder();
}

void Scene_Tool::update()
{
	Scene::update();

	MouseCheck();
}

void Scene_Tool::Enter()
{
	SetMenu(Core::GetInst()->GetWindowHandle(), m_menu);
	Core::GetInst()->SetResolution(Core::GetInst()->GetResolution(), true);

	Vector2 resolution = Core::GetInst()->GetResolution();
	Camera::GetInst()->SetLookAt(resolution / 2.f);

	Vector2 look = Camera::GetInst()->GetLookAt();
	Vector2 diff = Camera::GetInst()->GetDiff();

	Collider::SetLook(true);

	TileLoad(0, 0);
}

void Scene_Tool::Exit()
{
	SetMenu(Core::GetInst()->GetWindowHandle(), nullptr);
	Core::GetInst()->SetResolution(Core::GetInst()->GetResolution(), false);
}

void Scene_Tool::SaveMapSelect()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
	strMapFilePath += L"Data\\Map";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0¸Ê\0*.map\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring contentPath = CONTENT_PATH;

		wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
		SaveMap(relativePath);
	}
}

void Scene_Tool::LoadMapSelect()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
	strMapFilePath += L"Data\\Map";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0¸Ê\0*.map\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring contentPath = CONTENT_PATH;

		wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
		LoadMap(relativePath);
	}
}

void Scene_Tool::SaveMonsterSelect()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
	strMapFilePath += L"Data\\Map";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0¸ó½ºÅÍ\0*.monster\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring contentPath = CONTENT_PATH;

		wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
		SaveMonster(relativePath);
	}
}

void Scene_Tool::LoadMonsterSelect()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
	strMapFilePath += L"Data\\Map";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0¸ó½ºÅÍ\0*.monster\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring contentPath = CONTENT_PATH;

		wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
		LoadMonster(relativePath);
	}
}

void Scene_Tool::TextureLoad()
{
	if (SceneManager::GetInst()->GetObjectByName(L"AnimTexture", GROUP_TYPE::UI) == nullptr)
	{
		Vector2 resolution = Core::GetInst()->GetResolution();

		ImageUI* image = new ImageUI;
		image->LoadTexture();
		if (nullptr == image->GetTexture())
		{
			delete image;
			return;
		}

		DeleteAll();

		if (image->GetTexture()->GetWidth() > 600.f || image->GetTexture()->GetHeight() > 600.f)
		{
			image->SetPos(Vector2(resolution.x - 600.f, 0.f));
			image->SetScale(Vector2(600.f, 600.f));
		}
		else
		{
			image->SetPos(Vector2(resolution.x - (float)(image->GetTexture()->GetWidth()), 0.f));
			image->SetScale(Vector2((float)image->GetTexture()->GetWidth(), (float)image->GetTexture()->GetHeight()));
		}
		
		image->SetUIType(UI_TYPE::SHOW_TEXTURE);
		image->SetName(L"AnimTexture");

		HollowBox* box = new HollowBox;
		box->SetPos(Vector2(30.f, 0.f));
		box->SetScale(Vector2(50.f, 50.f));
		box->SetUIType(UI_TYPE::ABSTRACT_BOX);
		box->SetImageUI(image);
		box->SetName(L"AbstractBox");
		image->AddChild(box);

		AddObj(image, GROUP_TYPE::UI);

		NPC* sample = new NPC;
		sample->SetPos(Vector2((resolution.x - 600.f) / 2.f, resolution.y / 2.f));
		sample->CreateCollider();
		sample->SetName(L"Sample");
		sample->SetAnimAdjust(true);
		AddObj(sample, GROUP_TYPE::DEFAULT);

		PanelUI* pos = new PanelUI;
		pos->SetPos(Vector2(resolution.x - 600.f, 600.f));
		pos->SetScale(Vector2(600.f, resolution.y - 600.f));
		pos->SetBrush(BRUSH_TYPE::GRAY);
		pos->SetName(L"BoxPos");
		AddObj(pos, GROUP_TYPE::UI);

		HollowBox* texturePos = new HollowBox;
		texturePos->SetPos(Vector2(0.f, 0.f));
		texturePos->SetScale(Vector2(resolution.x - 600.f, resolution.y / 2.f));
		texturePos->SetIsMove(false);
		AddObj(texturePos, GROUP_TYPE::BACKGROUND);

		texturePos = texturePos->Clone();
		texturePos->SetScale(Vector2((resolution.x - 600.f) / 2.f, resolution.y));
		AddObj(texturePos, GROUP_TYPE::BACKGROUND);
	}
	else
	{
		Texture* tex = SceneManager::GetInst()->GetObjectByName(L"AnimTexture")->GetTexture();
		
		((ImageUI*)SceneManager::GetInst()->GetObjectByName(L"AnimTexture"))->LoadTexture();
	}
}

void Scene_Tool::TileLoad(int _col, int _row)
{
	if (SceneManager::GetInst()->GetObjectByName(L"MapTool", GROUP_TYPE::UI) == nullptr)
	{
		DeleteAll();

		CreateTile(_col, _row);

		Vector2 resolution = Core::GetInst()->GetResolution();

		PanelUI* mapTool = new PanelUI;
		mapTool->SetPos(Vector2(resolution.x - 600.f, 0.f));
		mapTool->SetScale(Vector2(600.f, resolution.y));
		mapTool->SetBrush(BRUSH_TYPE::GRAY);
		mapTool->SetPen(PEN_TYPE::HOLLOW);
		mapTool->SetUIType(UI_TYPE::TILE);
		mapTool->SetPadding(32.f, 100.f, 32.f, 300.f);
		mapTool->SetName(L"MapTool");
		mapTool->ToolImageSetting(L"TileSet", L"Texture\\TileSet.bmp");

		ImageUI* button = new ImageUI;
		button->SetPos(Vector2(100.f, 500.f));
		button->SetScale(Vector2(200.f, 50.f));
		button->SetUIType(UI_TYPE::CHANGE_RENDER_MODE);
		mapTool->AddChild(button);

		button = button->Clone();
		button->SetPos(Vector2(400.f, 500.f));
		button->SetScale(Vector2(50.f, 50.f));
		button->SetUIType(UI_TYPE::CREATE_COLLIDER);
		mapTool->AddChild(button);

		button = button->Clone();
		button->SetPos(Vector2(100.f, 600.f));
		button->SetScale(Vector2(100.f, 50.f));
		button->SetUIType(UI_TYPE::SET_BOUNDARY);
		mapTool->AddChild(button);

		AddObj(mapTool, GROUP_TYPE::UI);
	}
	else
	{
		CreateTile(_col, _row);
	}
}

void Scene_Tool::CreateObjUI()
{
	UI* check = (UI*)SceneManager::GetInst()->GetObjectByName(L"ObjectUI", GROUP_TYPE::UI);
	if (check == nullptr)
	{
		PanelUI* panel = new PanelUI;
		panel->SetPos(Vector2(700.f, 0.f));
		panel->SetScale(Vector2(500.f, 700.f));
		panel->SetName(L"ObjectUI");
		panel->SetIsMove(true);

		PanelUI* exit = new PanelUI;
		exit->SetPos(Vector2(panel->GetScale().x - 30.f, 0.f));
		exit->SetScale(Vector2(30.f, 30.f));
		exit->SetBrush(BRUSH_TYPE::BLACK);
		exit->SetPen(PEN_TYPE::RED);
		exit->SetName(L"Exit");
		exit->SetExit(true);
		panel->AddChild(exit);

		panel->ObjUISetting(Vector2(70.f, 50.f), L"Footman", L"Texture\\Character\\footman.bmp", GROUP_TYPE::MONSTER);
		panel->ObjUISetting(Vector2(250.f, 50.f), L"HeavyInfantry", L"Texture\\Character\\heavyInfantry.bmp", GROUP_TYPE::MONSTER);
		panel->ObjUISetting(Vector2(20.f, 350.f), L"BossBody", L"Texture\\Character\\boss_body.bmp", GROUP_TYPE::MONSTER);
		panel->ObjUISetting(Vector2(20.f, 150.f), L"BossLeftHand", L"Texture\\Character\\boss_left_hand.bmp", GROUP_TYPE::MONSTER);
		panel->ObjUISetting(Vector2(250.f, 150.f), L"BossRightHand", L"Texture\\Character\\boss_right_hand.bmp", GROUP_TYPE::MONSTER);

		AddObj(panel, GROUP_TYPE::UI);
	}
	else
	{
		UIManager::GetInst()->MoveFocusedUI(check);
	}
}

void Scene_Tool::CreateBackground(const wstring& _name, RENDER_MODE _mode)
{
	Background* background = new Background;
	background->SetPos(Vector2(0.f, 0.f));
	background->LoadTexture();
	if (background->GetTexture() == nullptr)
		return;
	background->SetRenderMode(_mode);
	background->SetName(_name);
	AddObj(background, GROUP_TYPE::BACKGROUND);
	
}

void Scene_Tool::CreateMonster(const wstring& _name, Texture* _texture, MONSTER_GROUP _type)
{
	wstring name = _name;
	name += L"00";

	if (SceneManager::GetInst()->GetObjectByName(name, GROUP_TYPE::MONSTER) == nullptr)
	{
		Monster* monster = new Monster;
		monster->SetPos(Vector2(0.f, 0.f));
		monster->SetTexture(_texture);
		monster->SetName(name);
		monster->SetMonsterType(_type);
		monster->CreateCollider();
		UINT width = monster->GetTexture()->GetWidth();
		UINT height = monster->GetTexture()->GetHeight();
		monster->GetCollider()->SetOffsetPos(Vector2(0.f, -1.f * (float)(height / 2)));
		monster->GetCollider()->SetScale(Vector2((float)width, (float)height));
		AddObj(monster, GROUP_TYPE::MONSTER);
	}
	else
	{
		int count = 1;
		while (SceneManager::GetInst()->GetObjectByName(name, GROUP_TYPE::MONSTER) != nullptr)
		{
			wchar_t temp[255] = L"";
			wchar_t num[32] = {};
			wcscpy_s(temp, name.c_str());

			for (size_t i = 0; i < wcslen(temp); ++i)
			{
				if (L'0' == temp[i])
				{
					temp[i] = L'\0';
					break;
				}
			}
			name = temp;
			name += L"0";
			_itow_s(count, num, 32);
			name += num;

			++count;
		}

		Monster* monster = new Monster;
		monster->SetPos(Vector2(0.f, 0.f));
		monster->SetTexture(_texture);
		monster->SetName(name);
		monster->SetMonsterType(_type);
		monster->CreateCollider();
		UINT width = monster->GetTexture()->GetWidth();
		UINT height = monster->GetTexture()->GetHeight();
		monster->GetCollider()->SetOffsetPos(Vector2(0.f, -1.f * (float)(height / 2)));
		monster->GetCollider()->SetScale(Vector2((float)width, (float)height));
		AddObj(monster, GROUP_TYPE::MONSTER);
	}
}

void Scene_Tool::MouseCheck()
{
	UI* focusedUI = UIManager::GetInst()->GetUI();

	if (nullptr != focusedUI)
	{
		const vector<UI*>& childUI = focusedUI->GetChild();
		for (int i = 0; i < childUI.size(); ++i)
		{
			if (childUI[i]->IsMouseOn())
				focusedUI = childUI[i];
		}
	}
	
	if (KEY_HOLD(KEY::LBUTTON) && (nullptr == focusedUI || !focusedUI->IsMouseOn()))
	{
		Vector2 mouse = MOUSE_POS;
		mouse = Camera::GetInst()->GetRealPos(mouse);

		vector<Obj*>& vectorTile = GetObjects(GROUP_TYPE::TILE);

		int row = (int)mouse.x / TILE_SIZE;
		int col = (int)mouse.y / TILE_SIZE;
		if (mouse.x < 0 || (int)GetRow() <= row || mouse.y < 0 || (int)GetCol() <= col)
		{
			return;
		}

		int idx = col * GetRow() + row;
		((Tile*)vectorTile[idx])->SetImgIdx(Tile::g_imgIdx);
		((Tile*)vectorTile[idx])->SetRenderMode(ImageUI::g_renderMode);
		((Tile*)vectorTile[idx])->SetTileType(ImageUI::g_tileType);
		if (ImageUI::g_bCreateCollider)
		{
			if (vectorTile[idx]->GetCollider() == nullptr)
			{
				vectorTile[idx]->CreateCollider();
				vectorTile[idx]->GetCollider()->SetOffsetPos(Vector2(TILE_SIZE / 2, TILE_SIZE / 2));
				vectorTile[idx]->GetCollider()->SetScale(Vector2(TILE_SIZE, TILE_SIZE));
			}
		}
	}

	if (KEY_TAP(KEY::RBUTTON) && (nullptr == focusedUI || !focusedUI->IsMouseOn()))
	{
		Vector2 mouse = MOUSE_POS;
		mouse = Camera::GetInst()->GetRealPos(mouse);

		vector<Obj*>& vectorTile = GetObjects(GROUP_TYPE::TILE);

		int row = (int)mouse.x / TILE_SIZE;
		int col = (int)mouse.y / TILE_SIZE;
		if (mouse.x < 0 || (int)GetRow() <= row || mouse.y < 0 || (int)GetCol() <= col)
		{
			return;
		}

		int idx = col * GetRow() + row;
		vectorTile[idx]->DeleteCollider();
	}
}

void Scene_Tool::SaveMap(const wstring& _relativePath)
{
	vector<Obj*>& vectorBackground = GetObjects(GROUP_TYPE::BACKGROUND);
	vector<Obj*>& vectorTile = GetObjects(GROUP_TYPE::TILE);

	wstring filePath = CONTENT_PATH;
	filePath += _relativePath;
	const wchar_t* test = wcsstr(_relativePath.c_str(), L".map");

	if (test == NULL)
	{
		filePath += L".map";
	}

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");

	assert(file);

	int col = GetCol();
	int row = GetRow();
	size_t count = vectorBackground.size();

	fwrite(&count, sizeof(count), 1, file);

	for (size_t i = 0; i < vectorBackground.size(); ++i)
	{
		((Background*)vectorBackground[i])->Save(file);
	}

	fwrite(&col, sizeof(int), 1, file);
	fwrite(&row, sizeof(int), 1, file);

	for (size_t i = 0; i < vectorTile.size(); ++i)
	{
		((Tile*)vectorTile[i])->Save(file);
	}

	fclose(file);
}

void Scene_Tool::SaveMonster(const wstring& _relativePath)
{
	vector<Obj*>& vectorMonster = GetObjects(GROUP_TYPE::MONSTER);

	wstring filePath = CONTENT_PATH;
	filePath += _relativePath;
	const wchar_t* test = wcsstr(_relativePath.c_str(), L".monster");

	if (test == NULL)
	{
		filePath += L".monster";
	}

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");

	assert(file);

	int count = (int)vectorMonster.size();
	fwrite(&count, sizeof(int), 1, file);


	for (size_t i = 0; i < vectorMonster.size(); ++i)
	{
		MONSTER_GROUP monType = ((Monster*)vectorMonster[i])->GetMonsterType();
		fwrite(&monType, sizeof(MONSTER_GROUP), 2, file);
		((Monster*)vectorMonster[i])->Save(file);
	}

	fclose(file);
}


LRESULT CALLBACK CreateTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			int col = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			int row = GetDlgItemInt(hWnd, IDC_EDIT2, nullptr, true);

			Scene* toolScene = SceneManager::GetInst()->GetCurScene();
			toolScene->DeleteObj(GROUP_TYPE::TILE);
			((Scene_Tool*)toolScene)->TileLoad(col, row);

			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK AdjustTileColliderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Scene* toolScene = SceneManager::GetInst()->GetCurScene();
	vector<Obj*>& vectorTile = toolScene->GetObjects(GROUP_TYPE::TILE);

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CHANGE_POS)
		{
			if (vectorTile.size() == 0)
				return(INT_PTR)TRUE;

			int col = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			int row = GetDlgItemInt(hWnd, IDC_EDIT2, nullptr, true);
			int x = GetDlgItemInt(hWnd, IDC_EDIT3, nullptr, true);
			int y = GetDlgItemInt(hWnd, IDC_EDIT4, nullptr, true);
			
			int idx = (col * toolScene->GetRow()) + row;

			if (vectorTile[idx]->GetCollider() == nullptr)
				return (INT_PTR)TRUE;

			vectorTile[idx]->GetCollider()->SetOffsetPos(Vector2(float(x + TILE_SIZE / 2.f), float(y + TILE_SIZE / 2.f)));
		}
		else if (LOWORD(wParam) == IDC_CHANGE_SCALE)
		{
			if (vectorTile.size() == 0)
				return (INT_PTR)TRUE;

			int col = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			int row = GetDlgItemInt(hWnd, IDC_EDIT2, nullptr, true);
			int x = GetDlgItemInt(hWnd, IDC_EDIT5, nullptr, true);
			int y = GetDlgItemInt(hWnd, IDC_EDIT6, nullptr, true);

			int idx = (col * toolScene->GetRow()) + row;

			if(vectorTile[idx]->GetCollider() == nullptr)
				return (INT_PTR)TRUE;

			vectorTile[idx]->GetCollider()->SetScale(Vector2(float(x), float(y)));
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}