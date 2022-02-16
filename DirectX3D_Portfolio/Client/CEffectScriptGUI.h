#pragma once
#include <Script\CEffectScript.h>
#include "CScriptGUI.h"

class CEffectScriptGUI :
    public CScriptGUI
{
public:
    virtual void update();
    void init();

public:
    CEffectScriptGUI();
    ~CEffectScriptGUI();
};