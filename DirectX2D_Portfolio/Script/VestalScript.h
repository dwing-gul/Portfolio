#pragma once
#include "PlayerScript.h"
class VestalScript :
    public PlayerScript
{
public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SkillTrigger(GameObject* _target);

private :
    void WideHeal(GameObject* _target, UINT _critical, int _heal, int _stress = 4);

    CLONE(VestalScript)

public:
    VestalScript();
    ~VestalScript();
};


