#include "pch.h"
#include "Core.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "PathManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "Device.h"
#include "Sound.h"

Core::Core() :
	m_hWnd(nullptr)
{
}

Core::~Core()
{
}

int Core::init(HWND _hWnd, Vector2 _windowResolution, Vector2 _renderResolution)
{
	m_hWnd = _hWnd;
	m_windowResolution = _windowResolution;

	RECT rt = { 0, 0, (int)m_windowResolution.x, (int)m_windowResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	if (FAILED(Device::GetInst()->init(m_hWnd, _renderResolution, true)))
	{
		MessageBox(nullptr, L"장치 초기화 실패", L"초기화 오류", MB_OK);
		return E_FAIL;
	}

	PathManager::GetInst()->init();
	TimeManager::GetInst()->init();
	FontManager::GetInst()->init();
	KeyManager::GetInst()->init();
	RenderManager::GetInst()->init();
	ResourceManager::GetInst()->init();
	SceneManager::GetInst()->init();

	return 0;
}

void Core::progress()
{
	TimeManager::GetInst()->update();
	KeyManager::GetInst()->update();
	ResourceManager::GetInst()->update();
	Sound::g_FMOD->update();

	SceneManager::GetInst()->progress();

	RenderManager::GetInst()->render();

	EventManager::GetInst()->update();
}
