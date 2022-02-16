#pragma once

class Texture;

class Core
{
	SINGLE(Core)
private :
	HWND		m_hWnd;
	HDC			m_hdc;
	POINT		m_resolution;

	Texture*	m_memTex;

	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];

public :
	int init(HWND _hWnd, POINT _resolution);
	void progress();
	void CreateGDIObject();

	void SetResolution(POINT _resolution, bool _bMenu);

	POINT GetResolution() { return m_resolution; }
	HWND GetWindowHandle() { return m_hWnd; }
	HDC GetCoreDC() { return m_hdc; }
	HPEN GetPen(PEN_TYPE _pen) { return m_arrPen[(UINT)_pen]; }
	HBRUSH GetBrush(BRUSH_TYPE _brush) { return m_arrBrush[(UINT)_brush]; }
};

