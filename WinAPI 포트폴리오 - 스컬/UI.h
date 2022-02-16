#pragma once
#include "Obj.h"

class UI :
    public Obj
{
private :
    vector<UI*> m_vectorChildUI;
    UI* m_parentUI;
    Vector2 m_finalPos;
    UI_TYPE m_uiType;

    Vector2 m_LButtonDownPos;
    Vector2 m_RButtonDownPos;
    bool m_bMouseOn;
    bool m_bLButtonDown;
    bool m_bRButtonDown;
    bool m_bLook;

protected :
    BRUSH_TYPE m_brush;
    PEN_TYPE m_pen;

public :
    virtual void update();
    virtual void lateUpdate();
    virtual void render(HDC _dc);

    void AddChild(UI* _child)
    {
        _child->m_parentUI = this;
        m_vectorChildUI.push_back(_child);
    }

    void SetBrush(BRUSH_TYPE _brush) { m_brush = _brush; }
    void SetPen(PEN_TYPE _pen) { m_pen = _pen; }
    void SetUIType(UI_TYPE _type) { m_uiType = _type; }
    void SetLook(bool _look) { m_bLook = _look; }
    
    const vector<UI*>& GetChild() { return m_vectorChildUI; }
    vector<UI*>& GetChildModifiable() { return m_vectorChildUI; }
    void DeleteChildUI();
    UI* GetChildByName(const wstring& _name);
    bool IsLook() { return m_bLook; }

    Vector2 GetFinalPos() { return m_finalPos; }
    Vector2 GetLButtonDownPos() { return m_LButtonDownPos; }
    UI* GetParentUI() { return m_parentUI; }
    UI_TYPE GetUIType() { return m_uiType; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLButtonDown() { return m_bLButtonDown; }
    bool IsRButtonDown() { return m_bRButtonDown; }

    virtual bool IsMouseOn(Vector2 _mousePos);
    virtual void MouseOn() { m_bMouseOn = true; }
    virtual void LButtonDown(Vector2 _mousePos) { m_bLButtonDown = true; m_LButtonDownPos = _mousePos; }
    virtual void LButtonUp(Vector2 _mousePos) {}
    virtual void LButtonClick(Vector2 _mousePos) {}

    virtual void RButtonDown(Vector2 _mousePos) { m_bRButtonDown = true; m_RButtonDownPos = _mousePos; }
    virtual void RButtonUp(Vector2 _mousePos) {}
    virtual void RButtonClick(Vector2 _mousePos);

    virtual UI* Clone() = 0;

public :
    UI();
    virtual ~UI();

    friend class UIManager;

protected :
    UI(const UI& _origin);
};

