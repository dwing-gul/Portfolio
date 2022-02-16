#pragma once
#include "MonsterScript.h"
class SkeletonCommonScript :
    public MonsterScript
{
public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SkillTrigger(GameObject* _target);
    virtual void SelectTarget(const vector<GameObject*>& _playerParty);

    CLONE(SkeletonCommonScript)

public:
    SkeletonCommonScript();
    ~SkeletonCommonScript();
};

