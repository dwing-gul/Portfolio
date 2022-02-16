#include "stdafx.h"
#include "ImageUI.h"
#include "Core.h"
#include "Texture.h"
#include "EventManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Scene_Tool.h"
#include "SceneManager.h"

bool ImageUI::g_bCreateCollider = false;
bool ImageUI::g_bTileBoundary = false;
RENDER_MODE ImageUI::g_renderMode = RENDER_MODE::TRANSPARENTBLT;
TILE_TYPE ImageUI::g_tileType = TILE_TYPE::NONE;

ImageUI::ImageUI() :
	m_LT(Vector2(0.f, 0.f)),
	m_tileIdx(0),
	m_bTile(false)
{
}

ImageUI::~ImageUI()
{
}

void ImageUI::update()
{
	if (!IsDead())
	{
		if (UI_TYPE::SHOW_TEXTURE == GetUIType())
		{
			float width = float(GetTexture()->GetWidth());
			float height = float(GetTexture()->GetHeight());
			if (KEY_HOLD(KEY::LEFT))
			{
				m_LT.x -= 300.f * DELTATIME;
				if (m_LT.x < 0.f)
					m_LT.x = 0.f;
			}

			if (KEY_HOLD(KEY::RIGHT))
			{
				m_LT.x += 300.f * DELTATIME;
				if (width > 600.f)
				{
					if (m_LT.x > width - 600.f)
						m_LT.x = width - 600.f;
				}
				else
				{
					m_LT.x = 0.f;
				}
			}

			if (KEY_HOLD(KEY::UP))
			{
				m_LT.y -= 300.f * DELTATIME;
				if (m_LT.y < 0.f)
					m_LT.y = 0.f;
			}

			if (KEY_HOLD(KEY::DOWN))
			{
				m_LT.y += 300.f * DELTATIME;
				if (height > 600.f)
				{
					if (m_LT.y > height - 600.f)
						m_LT.y = height - 600.f;
				}
				else
				{
					m_LT.y = 0.f;
				}

			}
		}
	}

	UI::update();
}

void ImageUI::render(HDC _dc)
{
	Texture* tex = GetTexture();

	if (nullptr == tex)
	{
		HPEN pen = Core::GetInst()->GetPen(m_pen);
		HBRUSH brush = Core::GetInst()->GetBrush(m_brush);

		if (UI_TYPE::CREATE_COLLIDER == GetUIType())
		{
			if (g_bCreateCollider)
				pen = Core::GetInst()->GetPen(PEN_TYPE::RED);
			else
				pen = Core::GetInst()->GetPen(PEN_TYPE::GREEN);
			brush = Core::GetInst()->GetBrush(BRUSH_TYPE::HOLLOW);
		}

		HPEN prevPen = (HPEN)SelectObject(_dc, pen);
		HBRUSH prevBrush = (HBRUSH)SelectObject(_dc, brush);

		Rectangle(_dc, int(GetFinalPos().x), int(GetFinalPos().y), int(GetFinalPos().x + GetScale().x), int(GetFinalPos().y + GetScale().y));

		SelectObject(_dc, prevBrush);
		SelectObject(_dc, prevPen);

		if (UI_TYPE::CHANGE_RENDER_MODE == GetUIType())
		{
			SetTextAlign(_dc, TA_CENTER);
			if (RENDER_MODE::ALPHABLEND == g_renderMode)
				TextOut(_dc, int(GetFinalPos().x) + 100, int(GetFinalPos().y) + 15, L"AlphaBlend", (int)wcslen(L"AlphaBlend"));
			else
				TextOut(_dc, int(GetFinalPos().x) + 100, int(GetFinalPos().y) + 15, L"BitBlt", (int)wcslen(L"BitBlt"));
		}
		else if (UI_TYPE::SET_BOUNDARY == GetUIType())
		{
			SetTextAlign(_dc, TA_CENTER);
			if (TILE_TYPE::BOUNDARY == g_tileType)
				TextOut(_dc, int(GetFinalPos().x) + 55, int(GetFinalPos().y) + 15, L"Boundary", (int)wcslen(L"Boundary"));
			else if (TILE_TYPE::NONE == g_tileType)
				TextOut(_dc, int(GetFinalPos().x) + 55, int(GetFinalPos().y) + 15, L"None", (int)wcslen(L"None"));
			else if (TILE_TYPE::GROUND == g_tileType)
				TextOut(_dc, int(GetFinalPos().x) + 55, int(GetFinalPos().y) + 15, L"Ground", (int)wcslen(L"Ground"));
			else if (TILE_TYPE::PLATFORM == g_tileType)
				TextOut(_dc, int(GetFinalPos().x) + 55, int(GetFinalPos().y) + 15, L"Platform", (int)wcslen(L"Platform"));
		}
	}
	else
	{
		Vector2 pos = GetFinalPos();
		UINT width = tex->GetWidth();
		UINT height = tex->GetHeight();

		BitBlt(_dc, (int)pos.x, (int)pos.y, (int)GetScale().x, (int)GetScale().y, tex->GetTextureDC(), (int)m_LT.x, (int)m_LT.y, SRCCOPY);
	}

	const vector<UI*>& vectorChildUI = GetChild();
	for (size_t i = 0; i < vectorChildUI.size(); ++i)
	{
		vectorChildUI[i]->render(_dc);
	}
}

void ImageUI::LButtonClick(Vector2 _mousePos)
{
	UI_TYPE type = GetUIType();
	if (UI_TYPE::TILE == type)
	{
		SelectTile(m_tileIdx);
	}
	else if (UI_TYPE::CHANGE_RENDER_MODE == type)
	{
		if (RENDER_MODE::ALPHABLEND == g_renderMode)
			g_renderMode = RENDER_MODE::BITBLT;
		else
			g_renderMode = RENDER_MODE::ALPHABLEND;
	}
	else if (UI_TYPE::CREATE_COLLIDER == type)
	{
		if (g_bCreateCollider)
			g_bCreateCollider = false;
		else
			g_bCreateCollider = true;
	}
	else if (UI_TYPE::SET_BOUNDARY == type)
	{
		if (TILE_TYPE::GROUND == g_tileType)
			g_tileType = TILE_TYPE::PLATFORM;
		else if (TILE_TYPE::PLATFORM == g_tileType)
			g_tileType = TILE_TYPE::BOUNDARY;
		else if (TILE_TYPE::BOUNDARY == g_tileType)
			g_tileType = TILE_TYPE::NONE;
		else if (TILE_TYPE::NONE == g_tileType)
			g_tileType = TILE_TYPE::GROUND;
	}
	else if (UI_TYPE::CREATE_OBJECT == type)
	{
		Scene* curScene = SceneManager::GetInst()->GetCurScene();
		wstring name = GetName();
		Texture* tex = GetTexture();

		if (name == L"Footman")
		{
			((Scene_Tool*)curScene)->CreateMonster(name, tex, MONSTER_GROUP::FOOTMAN);
		}
		else if (name == L"HeavyInfantry")
		{
			((Scene_Tool*)curScene)->CreateMonster(name, tex, MONSTER_GROUP::HEAVYINFANTRY);
		}
		else if (name == L"BossBody")
		{
			((Scene_Tool*)curScene)->CreateMonster(name, tex, MONSTER_GROUP::BOSS_BODY);
		}
		else if (name == L"BossLeftHand")
		{
			((Scene_Tool*)curScene)->CreateMonster(name, tex, MONSTER_GROUP::BOSS_LEFT_HAND);
		}
		else if (name == L"BossRightHand")
		{
			((Scene_Tool*)curScene)->CreateMonster(name, tex, MONSTER_GROUP::BOSS_RIGHT_HAND);
		}
	}
}

void ImageUI::LoadTexture()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
	strMapFilePath += L"Texture";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0텍스쳐\0*.bmp\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring contentPath = CONTENT_PATH;

		wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
		Load(relativePath);
	}
}

void ImageUI::Load(const wstring& _filePath)
{
	wchar_t temp[255] = {};
	wchar_t* fileName = {};
	int pos = 0;

	for (size_t i = _filePath.length(); i > 0; --i)
	{
		if (L'\\' == _filePath[i])
		{
			pos = (int)i + 1;
			break;
		}
	}

	for (int i = 0; i <= pos && pos < _filePath.length(); ++i)
	{
		temp[i] = _filePath[pos];
		++pos;
	}

	fileName = wcstok_s(temp, L".", &fileName);

	CreateTexture(fileName, _filePath);
}

void ImageUI::SelectTile(int _imgIdx)
{
	tEvent uiEvent = {};
	uiEvent.eType = EVENT_TYPE::SELECT_TILE;
	uiEvent.lParam = (DWORD_PTR)_imgIdx;

	EventManager::GetInst()->AddEvent(uiEvent);
}