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
	void MoveFocusedUI(CGameObject* _targetUI); // UI가 다른 UI에 가려져서 선택할 수 없을 때 쓰는 함수
	bool IsFocused() { return m_bFocused; }

private:
	void UIEvent(CGameObject* _parentUI);
	CGameObject* GetFocusedUI();
	CGameObject* GetTargetedUI(CGameObject* _parentUI);
};

