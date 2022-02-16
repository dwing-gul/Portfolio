#include "stdafx.h"
#include "Core.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "PathManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "Texture.h"
#include "UIManager.h"
#include "Camera.h"

Core::Core() :
	m_resolution{},
	m_hWnd(0),
	m_hdc(0)
{
}

Core::~Core()
{
	ReleaseDC(m_hWnd, m_hdc);

	for (int i = 0; i < (int)PEN_TYPE::HOLLOW; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
}

int Core::init(HWND _hWnd, POINT _resolution)
{
	m_hWnd = _hWnd;

	SetResolution(_resolution, false);

	m_hdc = GetDC(m_hWnd);

	TimeManager::GetInst()->init();
	KeyManager::GetInst()->init();
	SceneManager::GetInst()->init();
	PathManager::GetInst()->init();
	Camera::GetInst()->init();
	SoundManager::GetInst()->init();

	m_memTex = ResourceManager::GetInst()->CreateTexture(L"MemTex", m_resolution.x, m_resolution.y);

	CreateGDIObject();

	return S_OK;
}

void Core::progress()
{
	TimeManager::GetInst()->update();
	KeyManager::GetInst()->update();
	SceneManager::GetInst()->progress();

	Camera::GetInst()->update();
	CollisionManager::GetInst()->update();
	UIManager::GetInst()->update();

	Rectangle(m_memTex->GetTextureDC(), -1, -1, m_resolution.x + 1, m_resolution.y + 1);

	SceneManager::GetInst()->render(m_memTex->GetTextureDC());
	Camera::GetInst()->render(m_memTex->GetTextureDC());

	BitBlt(m_hdc, 0, 0, m_resolution.x, m_resolution.y, m_memTex->GetTextureDC(), 0, 0, SRCCOPY);

	EventManager::GetInst()->update();
}

void Core::CreateGDIObject()
{
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::BLACK] = (HPEN)GetStockObject(BLACK_PEN);
	m_arrPen[(UINT)PEN_TYPE::WHITE] = (HPEN)GetStockObject(WHITE_PEN);
	m_arrPen[(UINT)PEN_TYPE::HOLLOW] = (HPEN)GetStockObject(NULL_PEN);

	m_arrBrush[(UINT)BRUSH_TYPE::WHITE] = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::GRAY] = (HBRUSH)GetStockObject(GRAY_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

void Core::SetResolution(POINT _resolution, bool _bMenu)
{
	m_resolution = _resolution;

	RECT rt = { 0, 0, m_resolution.x, m_resolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}
