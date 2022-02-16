#include "stdafx.h"
#include "Background.h"
#include "Camera.h"
#include "Texture.h"
#include "SceneManager.h"
#include "Scene_Tool.h"
#include "resource.h"

Background::Background() :
	m_renderMode(RENDER_MODE::BITBLT),
	m_magnification{ 1, 1 }
{
}

Background::~Background()
{
}

void Background::update()
{
}

void Background::render(HDC _dc)
{
	Vector2 renderPos = Camera::GetInst()->GetRenderPos(GetPos());
	Texture* tex = GetTexture();
	UINT width = tex->GetWidth();
	UINT height = tex->GetHeight();

	if (RENDER_MODE::ALPHABLEND == m_renderMode)
	{
		BLENDFUNCTION blend = {};
		blend.AlphaFormat = AC_SRC_ALPHA;
		blend.BlendFlags = 0;
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;

		AlphaBlend(_dc, (int)renderPos.x, (int)renderPos.y, width * m_magnification.x, height * m_magnification.y,
			tex->GetTextureDC(), 0, 0, width, height, blend);
	}
	else if (RENDER_MODE::BITBLT == m_renderMode)
	{
		BitBlt(_dc, (int)renderPos.x, (int)renderPos.y, width, height, tex->GetTextureDC(), 0, 0, SRCCOPY);
	}
}

void Background::Save(FILE* _file)
{
	// 저장해야될 정보 : 렌더링 모드, 텍스쳐
	fwrite(&m_renderMode, sizeof(RENDER_MODE), 2, _file);
	SaveWString(GetTexture()->GetKey(), _file);
	SaveWString(GetTexture()->GetRelativePath(), _file);
	Vector2 pos = GetPos();
	fwrite(&pos, sizeof(Vector2), 2, _file);
}

void Background::Load(FILE* _file)
{
	fread(&m_renderMode, sizeof(RENDER_MODE), 2, _file);

	wstring key, relativePath;

	LoadWString(key, _file);
	LoadWString(relativePath, _file);
	CreateTexture(key, relativePath);

	fread(&m_pos, sizeof(Vector2), 2, _file);
}

LRESULT CALLBACK CreateBackgroundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();
	size_t count = curScene->GetObjects(GROUP_TYPE::BACKGROUND).size();
	wstring name = L"Background";
	wchar_t temp[10] = {};
	_itow_s((int)count, temp, 10);
	name += temp;
	static RENDER_MODE mode = RENDER_MODE::BITBLT;

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case IDC_BITBLT :
				mode = RENDER_MODE::BITBLT;
				break;
			case IDC_ALPHABLEND :
				mode = RENDER_MODE::ALPHABLEND;
				break;
			}
		}

		if (LOWORD(wParam) == IDOK)
		{
			((Scene_Tool*)curScene)->CreateBackground(name, mode);
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

LRESULT CALLBACK AdjustBackgroundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			int x = GetDlgItemInt(hWnd, IDC_EDIT1, nullptr, true);
			int y = GetDlgItemInt(hWnd, IDC_EDIT2, nullptr, true);
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = L"Background";
			name += temp;

			Background* bg = (Background*)SceneManager::GetInst()->GetObjectByName(name, GROUP_TYPE::BACKGROUND);

			if(nullptr != bg)
				bg->SetMagnification(POINT{ x, y });

			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_DELETE)
		{
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = L"Background";
			name += temp;

			Background* bg = (Background*)SceneManager::GetInst()->GetObjectByName(name, GROUP_TYPE::BACKGROUND);
			SceneManager::GetInst()->DeleteObjectByName(name, GROUP_TYPE::BACKGROUND);
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_MOVE_POS)
		{
			int x = GetDlgItemInt(hWnd, IDC_EDIT4, nullptr, true);
			int y = GetDlgItemInt(hWnd, IDC_EDIT5, nullptr, true);
			wchar_t temp[255] = {};
			GetDlgItemText(hWnd, IDC_EDIT3, temp, 255);
			wstring name = L"Background";
			name += temp;

			Background* bg = (Background*)SceneManager::GetInst()->GetObjectByName(name, GROUP_TYPE::BACKGROUND);

			if (nullptr != bg)
				bg->SetPos(Vector2(x, y));

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