#pragma once
#include <Script\CLayerUpdateBox.h>
#include "CScriptGUI.h"


class CLayUpdateBoxGUI :
    public CScriptGUI
{
public:
    virtual void update();
public:
    CLayUpdateBoxGUI();
    ~CLayUpdateBoxGUI();
};