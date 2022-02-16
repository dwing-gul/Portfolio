#pragma once

class TextUIScript;

class TextUIManager :
	public Singleton<TextUIManager>
{
	SINGLE(TextUIManager)
private :
	vector<TextUIScript*>	m_vectorTextUI;

public :
	void init();
	void progress();

private :
	void AddTextUI();
	void SetTarget(TextUIScript* _textUI);
	void PlayerTextUI(TextUIScript* _textUI);
	void MonsterTextUI(TextUIScript* _textUI);
	void ItemTextUI(TextUIScript* _textUI);
};

