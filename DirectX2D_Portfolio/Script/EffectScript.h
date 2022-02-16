#pragma once
#include <Engine/Script.h>

class GameObject;

class EffectScript :
    public Script
{
private :
    GameObject* m_owner;
    bool        m_bChange;
    float       m_originXPos;
    float       m_originYPos;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    CLONE(EffectScript)

public:
    EffectScript();
    ~EffectScript();
};

