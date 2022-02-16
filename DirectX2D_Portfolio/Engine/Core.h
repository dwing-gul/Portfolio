#pragma once

class Core :
	public Singleton<Core>
{
	SINGLE(Core)
private:
	HWND	m_hWnd;
	Vector2 m_windowResolution;

public:
	int init(HWND _hWnd, Vector2 _windowResolution, Vector2 _renderResolution);
	void progress();

	HWND GetWindowHandle() { return m_hWnd; }
	Vector2 GetWindowResolution() { return m_windowResolution; }
};

