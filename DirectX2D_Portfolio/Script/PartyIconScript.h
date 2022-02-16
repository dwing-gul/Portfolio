#pragma once
#include <Engine/Script.h>

class SlotScript;

class PartyIconScript :
    public Script
{
private :
    Ptr<Prefab> m_prefab;
    Vector3     m_originPos;
    SlotScript* m_slot;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    Ptr<Prefab> GetPrefab() { return m_prefab; }
    void SetSlot(SlotScript* _slot) { m_slot = _slot; }

    CLONE(PartyIconScript)

public:
    PartyIconScript();
    ~PartyIconScript();
};

