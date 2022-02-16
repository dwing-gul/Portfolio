#pragma once

class CCore :
	public CSingleton<CCore>
{
	SINGLE(CCore);

private :
	HWND m_hWnd;
	Vec2 m_windowResolution;

public :
	int init(HWND _hWnd, Vec2 _windowResolution, Vec2 _renderResolution);
	void progress();
	HWND GetWindowHandle() { return m_hWnd; }
	Vec2 GetWindowResolution() { return m_windowResolution; }
};

