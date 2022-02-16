#pragma once

class CGUI;

class CImGuiManager :
	public CSingleton<CImGuiManager>
{
	SINGLE(CImGuiManager)
private :
	map<string, CGUI*> m_mapGUI;
	bool m_bDemo;

public :
	void init();
	void process();
	void SetDemo(bool _bDemo) { m_bDemo = _bDemo; }

private :
	void update();
	void render();
	void CreateGUI();

public :
	CGUI* FindGUI(const string& _key);
};

