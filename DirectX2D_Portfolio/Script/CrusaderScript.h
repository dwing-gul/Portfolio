#pragma once
#include "PlayerScript.h"

class GameObject;

class CrusaderScript :
    public PlayerScript
{
public :
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SkillTrigger(GameObject* _target); 

private :
    void WideAttack(GameObject* _target, UINT _accuracy, UINT _critical, int _damage);

    CLONE(CrusaderScript)

public :
    CrusaderScript();
    ~CrusaderScript();
};

