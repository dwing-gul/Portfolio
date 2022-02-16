#pragma once
#include <Engine/Script.h>

class TownObjectScript :
    public Script
{
private :
    bool m_bStoreOpen;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetStoreOpen(bool _open) { m_bStoreOpen = _open; }

    CLONE(TownObjectScript)

public:
    TownObjectScript();
    ~TownObjectScript();
};

