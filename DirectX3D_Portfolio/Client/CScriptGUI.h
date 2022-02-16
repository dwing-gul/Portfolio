#pragma once
#include "CComponentGUI.h"
#include <Script/CScriptManager.h>
#include <Engine/CScript.h>

class CScriptGUI :
    public CComponentGUI
{
private :
    CScript*        m_script;
    SCRIPT_TYPE     m_scriptType;
    string          m_scriptName;
    bool            m_bSubScript;

public:
    virtual void update() = 0;
    CScript* GetScript() { return m_script; }

protected :
    virtual bool Start();

private :
    void SubScript(SCRIPT_TYPE _type);

public:
    CScriptGUI(SCRIPT_TYPE _type);
    ~CScriptGUI();
};

