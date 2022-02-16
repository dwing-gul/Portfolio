#pragma once
#include "UI.h"

class PanelUI :
    public UI
{
private :
    Vector2 m_diff;
    float m_leftPadding;
    float m_topPadding;
    float m_rightPadding;
    float m_bottomPadding;

    Vector2 m_insidePadding;
    SUBJECT_TYPE m_subjectType;
    ITEM_TYPE m_itemType;
    SKUL m_skulType;

    UINT m_UICountX;
    UINT m_UICountY;

    Vector2 m_UIScale;

    bool m_bMove;
    bool m_bExit;
    bool m_bBitBlt; // 그리기 모드가 BitBlt인가?

    int m_itemCount; // 인벤토리의 아이템의 갯수

public :
    void SetPadding(float _left, float _top, float _right, float _bottom)
    {
        m_leftPadding = _left;
        m_topPadding = _top;
        m_rightPadding = _right;
        m_bottomPadding = _bottom;
    }

    void SetIsMove(bool _move) { m_bMove = _move; }
    void SetExit(bool _exit) { m_bExit = _exit; }
    void SetItemType(ITEM_TYPE _type) { m_itemType = _type; }
    void SetSubjectType(SUBJECT_TYPE _type) { m_subjectType = _type; }
    void SetSkulType(SKUL _type) { m_skulType = _type; }
    void IsBitBlt(bool _render) { m_bBitBlt = _render; }
    void AddItemCount() { ++m_itemCount; }
    void SubItemCount() { --m_itemCount; }
    
    float GetLeftPadding() { return m_leftPadding; }
    float GetRightPadding() { return m_rightPadding; }
    float GetTopPadding() { return m_topPadding; }
    float GetBottomPadding() { return m_bottomPadding; }
    SUBJECT_TYPE GetSubjectType() { return m_subjectType; }
    ITEM_TYPE GetItemType() { return m_itemType; }
    SKUL GetSkulType() { return m_skulType; }
    Vector2 GetInsidePadding() { return m_insidePadding; }
    int GetItemCount() { return m_itemCount; }

    virtual void update();
    virtual void render(HDC _dc);
    virtual void LButtonDown(Vector2 _mousePos);
    virtual void LButtonClick(Vector2 _mousePos);

    void ToolImageSetting(const wstring& _key, const wstring& _relativePath);
    void ObjUISetting(Vector2 _pos, const wstring& _key, const wstring& _relativePath, GROUP_TYPE _type);

    CLONE(PanelUI);

public :
    PanelUI();
    virtual ~PanelUI();
};

