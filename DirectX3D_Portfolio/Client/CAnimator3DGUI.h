#pragma once
#include "CComponentGUI.h"

class CAnimation3D;

class CAnimator3DGUI :
    public CComponentGUI
{
private:
    bool    m_bRepeat;
    int    m_repeatCount;

public:
    virtual void update();

public:
    CAnimator3DGUI();
    ~CAnimator3DGUI();
};

