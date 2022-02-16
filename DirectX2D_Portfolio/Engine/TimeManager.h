#pragma once

class TimeManager :
	public Singleton<TimeManager>
{
	SINGLE(TimeManager);
private :
	LARGE_INTEGER	m_frequency;
	LARGE_INTEGER	m_prevFrame;
	LARGE_INTEGER	m_curQueryFrame;
	LARGE_INTEGER	m_prevQueryFrame;
	int				m_frameCount;
	int				m_FPS;
	float			m_deltaTime;
	float			m_pauseTime;
	bool			m_bPause;

public :
	float GetDeltaTime() { return m_deltaTime; }
	int GetFPS() { return m_FPS; }

	void Pause();

public :
	void init();
	void update();
};

