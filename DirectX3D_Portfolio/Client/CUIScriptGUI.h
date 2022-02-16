#pragma once
#include "CScriptGUI.h"
class CUIScriptGUI :
    public CScriptGUI
{
public:
    virtual void update() = 0;

    void SettingUIType();

public:
    CUIScriptGUI(SCRIPT_TYPE _type);
    ~CUIScriptGUI();
};

