#pragma once
#include "CComponentGUI.h"

class CCollider3DGUI :
    public CComponentGUI
{

public:
    virtual void update();

public:
    CCollider3DGUI();
    ~CCollider3DGUI();
};

