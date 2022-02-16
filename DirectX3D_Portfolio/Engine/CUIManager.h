#pragma once

class CGameObject;

class CUIManager :
	public CSingleton<CUIManager>
{
	SINGLE(CUIManager);
private:
	CGameObject* m_focusedUI;
	bool m_bFocused;

public:
	void update();
	CGameObject* GetUI() { return m_focusedUI; }
	void MoveFocusedUI(CGameObject* _targetUI); // UI�� �ٸ� UI�� �������� ������ �� ���� �� ���� �Լ�
	bool IsFocused() { return m_bFocused; }

private:
	void UIEvent(CGameObject* _parentUI);
	CGameObject* GetFocusedUI();
	CGameObject* GetTargetedUI(CGameObject* _parentUI);
};

