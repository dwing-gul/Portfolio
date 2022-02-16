#pragma once
#include "CComponentGUI.h"

class CCollider2DGUI :
    public CComponentGUI
{
public :
    virtual void update();

public :
    CCollider2DGUI();
    ~CCollider2DGUI();
};

