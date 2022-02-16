#pragma once
#include "MonsterScript.h"
class MadmanScript :
    public MonsterScript
{
public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SkillTrigger(GameObject* _target);
    virtual void SelectTarget(const vector<GameObject*>& _playerParty);

private :
    void WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, UINT _blight, int _damage);

    CLONE(MadmanScript)

public:
    MadmanScript();
    ~MadmanScript();
};

