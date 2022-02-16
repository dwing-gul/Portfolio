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
	void MoveFocusedUI(UI* _targetUI); // UI�� �ٸ� UI�� �������� ������ �� ���� �� ���� �Լ�

private :
	void UIEvent(UI* _parentUI);
	UI* GetFocusedUI();
	UI* GetTargetedUI(UI* _parentUI);
};

