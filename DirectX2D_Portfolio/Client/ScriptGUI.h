#pragma once
#include "ComponentGUI.h"
#include <Script/ScriptManager.h>
#include <Engine/Script.h>

class ScriptGUI :
    public ComponentGUI
{
private :
    Script*         m_script;
    SCRIPT_TYPE     m_scriptType;
    string          m_scriptName;
    bool            m_bSubScript;

public:
    virtual void update() = 0;
    Script* GetScript() { return m_script; }

protected :
    virtual bool Start();

private :
    void SubScript(SCRIPT_TYPE _type);

public:
    ScriptGUI(SCRIPT_TYPE _type);
    ~ScriptGUI();
};

