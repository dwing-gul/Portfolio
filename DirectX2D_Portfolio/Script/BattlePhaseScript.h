#pragma once
#include <Engine/Script.h>

class BattlePhaseScript :
    public Script
{
private :
    bool m_bBattlePhase;

public :
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetBattlePhase(bool _battle) { m_bBattlePhase = _battle; }
    bool IsBattlePhase() { return m_bBattlePhase; }

    CLONE(BattlePhaseScript)

public :
    BattlePhaseScript();
    ~BattlePhaseScript();
};

