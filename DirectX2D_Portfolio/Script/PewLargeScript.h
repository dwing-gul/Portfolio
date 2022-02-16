#pragma once
#include "MonsterScript.h"
class PewLargeScript :
    public MonsterScript
{
private:
    bool m_bDie;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SkillTrigger(GameObject* _target);
    virtual void SelectTarget(const vector<GameObject*>& _playerParty);

    CLONE(PewLargeScript)

public:
    PewLargeScript();
    ~PewLargeScript();
};

