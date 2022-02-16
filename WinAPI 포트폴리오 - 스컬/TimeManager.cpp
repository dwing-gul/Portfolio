#include "stdafx.h"
#include "TimeManager.h"
#include "Core.h"

TimeManager::TimeManager() :
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

TimeManager::~TimeManager()
{
}

void TimeManager::Pause()
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

void TimeManager::init()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_prevFrame);

	m_prevQueryFrame = m_prevFrame;
	m_curQueryFrame = m_prevFrame;
}

void TimeManager::update()
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
		SetWindowTextW(Core::GetInst()->GetWindowHandle(), szBuffer);
	}

	if (!m_bPause)
	{
		m_deltaTime = float(m_curQueryFrame.QuadPart - m_prevQueryFrame.QuadPart) / float(m_frequency.QuadPart);
		m_pauseTime = m_deltaTime;
	}
}
