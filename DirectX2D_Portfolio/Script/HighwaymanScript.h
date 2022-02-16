#pragma once
#include "PlayerScript.h"
class HighwaymanScript :
    public PlayerScript
{
public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SkillTrigger(GameObject* _target);

private :
    void WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, int _damage);

    CLONE(HighwaymanScript)

public:
    HighwaymanScript();
    ~HighwaymanScript();
};
