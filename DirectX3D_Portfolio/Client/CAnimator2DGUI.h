#pragma once
#include "CComponentGUI.h"

class CAnimation2D;

class CAnimator2DGUI :
    public CComponentGUI
{
private :
    CAnimation2D* m_selectedAnim;
    Vec2 m_frameSize;
    bool m_bFirst;

public:
    virtual void update();

private :
    void SetTexture(DWORD_PTR _data1, DWORD_PTR _data2);
    void AnimationLoad();
public:
    CAnimator2DGUI();
    ~CAnimator2DGUI();
};

