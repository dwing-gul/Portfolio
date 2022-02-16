#pragma once
#include <Script\CIGItemObjScript.h>
#include "CScriptGUI.h"

#pragma once
class CIGItemScriptGUI :
    public CScriptGUI
{
public:
    virtual void update();
    void init();

public:
    CIGItemScriptGUI();
    ~CIGItemScriptGUI();
};