#pragma once

class UI;

class UIManager
{
	SINGLE(UIManager)
private :
	UI* m_focusedUI;

public :
	void update();
	UI* GetUI() { return m_focusedUI; }
	void MoveFocusedUI(UI* _targetUI); // UI가 다른 UI에 가려져서 선택할 수 없을 때 쓰는 함수

private :
	void UIEvent(UI* _parentUI);
	UI* GetFocusedUI();
	UI* GetTargetedUI(UI* _parentUI);
};

