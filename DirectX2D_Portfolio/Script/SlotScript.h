#pragma once
#include <Engine/Script.h>

class PartyIconScript;

class SlotScript :
    public Script
{
private:
    PartyIconScript*    m_owner;
    Ptr<Prefab>         m_prefab;
    int                 m_position;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetOwner(PartyIconScript* _owner) { m_owner = _owner; }
    void SetPrefab(Ptr<Prefab> _prefab) { m_prefab = _prefab; }
    Ptr<Prefab> GetPrefab() { return m_prefab; }
    PartyIconScript* GetOwner() { return m_owner; }
    int GetSlotPosition() { return m_position; }

    CLONE(SlotScript)

public:
    SlotScript();
    ~SlotScript();
};

