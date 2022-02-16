#include "pch.h"
#include "CCore.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CEventManager.h"
#include "CRenderManager.h"
#include "CFontManager.h"
#include "CUIManager.h"
#include "CDevice.h"
#include "CSound.h"
#include "CInstancingBuffer.h"

CCore::CCore() :
	m_hWnd(nullptr)
{
}

CCore::~CCore()
{
}

int CCore::init(HWND _hWnd, Vec2 _windowResolution, Vec2 _renderResolution)
{
	m_hWnd = _hWnd;
	m_windowResolution = _windowResolution;

	RECT rect = { 0, 0, (int)m_windowResolution.x, (int)m_windowResolution.y };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rect.right - rect.left, rect.bottom - rect.top, 0);

	if (FAILED(CDevice::GetInst()->init(m_hWnd, _renderResolution, true)))
	{
		MessageBox(nullptr, L"장치 초기화 실패", L"초기화 오류", MB_OK);
		return E_FAIL;
	}

	CPathManager::GetInst()->init();
	CTimeManager::GetInst()->init();
	CFontManager::GetInst()->init();
	CKeyManager::GetInst()->init();
	CResourceManager::GetInst()->init();
	CRenderManager::GetInst()->init();
	CSceneManager::GetInst()->init();
	CInstancingBuffer::GetInst()->init();

	return S_OK;
}

void CCore::progress()
{
	CTimeManager::GetInst()->update();
	CKeyManager::GetInst()->update();
	CResourceManager::GetInst()->update();
	CSound::g_FMOD->update();
	CUIManager::GetInst()->update();

	CSceneManager::GetInst()->progress();

	CRenderManager::GetInst()->render();

	CEventManager::GetInst()->update();
}
