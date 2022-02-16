#pragma once
#include <Script\CMonsterScript.h>
#include "CScriptGUI.h"


class CMonsterScriptGUI :
    public CScriptGUI
{
public:
    virtual void update();

    void init();

private:
    string m_AnimName[(UINT)MONSTER_ANIMCLIP::END];

public:
    CMonsterScriptGUI();
    ~CMonsterScriptGUI();
};