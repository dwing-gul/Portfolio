#pragma once
#include "PlayerScript.h"
class PlagueDoctorScript :
    public PlayerScript
{
public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SkillTrigger(GameObject* _target);

private:
    void WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, UINT _debuff, int _damage);

    CLONE(PlagueDoctorScript)

public:
    PlagueDoctorScript();
    ~PlagueDoctorScript();
};