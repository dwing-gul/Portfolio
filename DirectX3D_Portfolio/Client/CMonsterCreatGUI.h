#pragma once
#include <Script\CMonsterCreateScript.h>
#include <Script\define.h>
#include "CScriptGUI.h"

#pragma once
class CMonsterCreatGUI :
    public CScriptGUI
{
public:
    virtual void update();
    string GetMonsterType(MONSTER_TYPE _type);
    string MonsterName[7];

    void init();

public:
    CMonsterCreatGUI();
    ~CMonsterCreatGUI();
};