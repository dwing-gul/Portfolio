#pragma once
#include <Script\CAIPointScript.h>
#include "CScriptGUI.h"

class CAIPointScriptGUI :
    public CScriptGUI
{
public:
    void init();

    virtual void update();

private:
    string m_pointType[(UINT)POINT_TYPE::HUNTING + 1];

public:
    CAIPointScriptGUI();
    ~CAIPointScriptGUI();
};