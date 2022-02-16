#pragma once
#include "CComponentGUI.h"

class CLight3DGUI :
    public CComponentGUI
{
private:
    Vec3    m_dir;
    bool    m_bFirst;

public:
    virtual void update();

public:
    CLight3DGUI();
    ~CLight3DGUI();
};