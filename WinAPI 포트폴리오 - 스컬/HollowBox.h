#pragma once
#include "UI.h"

class ImageUI;

class HollowBox :
    public UI
{
private :
    ImageUI* m_imageUI;
    Vector2 m_finalLT;
    Vector2 m_diff;

    bool m_bMove;

public :
    virtual void update();
    virtual void lateUpdate();
    virtual void render(HDC _dc);
    virtual void LButtonDown(Vector2 _mousePos);

    void SetImageUI(ImageUI* _imageUI) { m_imageUI = _imageUI; }
    void SetIsMove(bool _move) { m_bMove = _move; }
    Vector2 GetFinalLT() { return m_finalLT; }

    CLONE(HollowBox);

public :
    HollowBox();
    ~HollowBox();
};

