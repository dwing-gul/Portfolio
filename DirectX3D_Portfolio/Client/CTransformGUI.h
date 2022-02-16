#pragma once
#include "CComponentGUI.h"

class CTransformGUI :
    public CComponentGUI
{
public :
    virtual void update();

public :
    CTransformGUI();
    virtual ~CTransformGUI();
};

