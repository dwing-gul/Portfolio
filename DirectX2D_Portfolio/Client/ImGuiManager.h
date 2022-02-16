#pragma once

class GUI;

class ImGuiManager :
	public Singleton<ImGuiManager>
{
	SINGLE(ImGuiManager)
private :
	map<string, GUI*>	m_mapGUI;
	bool				m_bDemo;

public :
	void init();
	void progress();

	void SetDemo(bool _bDemo) { m_bDemo = _bDemo; }

private :
	void update();
	void render();
	void CreateGUI();

public :
	GUI* FindGUI(const string& _key);
};

