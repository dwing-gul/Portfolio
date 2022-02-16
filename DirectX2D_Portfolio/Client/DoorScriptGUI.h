#pragma once
#include "ScriptGUI.h"

class DoorScriptGUI :
    public ScriptGUI
{
public:
    virtual void update();

private :
    void SetDestination(DWORD_PTR _dw1, DWORD_PTR _dw2);

public:
    DoorScriptGUI();
    ~DoorScriptGUI();
};

