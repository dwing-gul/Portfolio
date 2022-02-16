#pragma once
#include "define.h"
#include <Engine\CScript.h>

class CMonsterCreateScript :
    public CScript
{
private:
    MONSTER_TYPE    m_monsterType;
    CPrefab*        m_bat;
    CPrefab*        m_bear;
    CPrefab*        m_boar;
    CPrefab*        m_chicken;
    CPrefab*        m_dog;
    CPrefab*        m_wolf;

public:
    void SetMonsterType(MONSTER_TYPE _type) { m_monsterType = _type; }
    MONSTER_TYPE GetMonsterType() { return m_monsterType; }

    void MonsterCreate();   // 테스트 함수

public:
    virtual void start();
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);
    
    CLONE(CMonsterCreateScript)

public:
    CMonsterCreateScript();
    ~CMonsterCreateScript();
};