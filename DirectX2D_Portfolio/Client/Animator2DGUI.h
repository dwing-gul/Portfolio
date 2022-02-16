#pragma once
#include "ComponentGUI.h"

class Animation2D;

class Animator2DGUI :
    public ComponentGUI
{
private :
    Animation2D* m_selectedAnim;
    Vector2 m_frameSize;
    bool m_bFirst;

public:
    virtual void update();

private :
    void SetTexture(DWORD_PTR _data1, DWORD_PTR _data2);

public:
    Animator2DGUI();
    ~Animator2DGUI();
};

