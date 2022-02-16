#include "pch.h"
#include "CTimeManager.h"
#include "CCore.h"

CTimeManager::CTimeManager() :
	m_frequency(),
	m_prevFrame(),
	m_curQueryFrame(), 
	m_prevQueryFrame(),
	m_deltaTime(0.f),
	m_pauseTime(0.f),
	m_frameCount(0),
	m_FPS(0),
	m_bPause(false)
{
}

CTimeManager::~CTimeManager()
{
}

void CTimeManager::Pause()
{
	if (!m_bPause)
	{
		m_deltaTime = 0; 
		m_bPause = true;
	}
	else
	{
		m_deltaTime = m_pauseTime;
		m_bPause = false;
	}
}

void CTimeManager::init()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_prevFrame);

	m_prevQueryFrame = m_prevFrame;
	m_curQueryFrame = m_prevFrame;
}

void CTimeManager::update()
{
	++m_frameCount;
	m_prevQueryFrame = m_curQueryFrame;
	QueryPerformanceCounter(&m_curQueryFrame);

	if (m_curQueryFrame.QuadPart - m_prevFrame.QuadPart > m_frequency.QuadPart)
	{
		QueryPerformanceFrequency(&m_frequency);
		m_prevFrame = m_curQueryFrame;
		m_FPS = m_frameCount;
		m_frameCount = 0;

		wchar_t szBuffer[255] = L"";
		wsprintf(szBuffer, L"FPS : %d", m_FPS);
		SetWindowTextW(CCore::GetInst()->GetWindowHandle(), szBuffer);
	}

	if (!m_bPause)
	{
		m_deltaTime = float(m_curQueryFrame.QuadPart - m_prevQueryFrame.QuadPart) / float(m_frequency.QuadPart);
		m_pauseTime = m_deltaTime;
	}

#ifdef _DEBUG
	if (m_deltaTime > 0.1666666f)
		m_deltaTime = 0.1666666f;
#endif

	g_global.deltaTime = m_deltaTime;
	g_global.accTime += m_deltaTime;
}
