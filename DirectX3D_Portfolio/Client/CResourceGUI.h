#pragma once
#include "CInspectorInfoGUI.h"

class CRes;

class CResourceGUI :
    public CInspectorInfoGUI
{
private :
    CRes* m_target;
    RESOURCE_TYPE m_resType;

public :
    void SetTarget(CRes* _target) { m_target = _target; }
    CRes* GetTarget() { return m_target; }

    virtual void update() = 0;

protected :
    virtual bool Start();
    virtual void End();

public :
    CResourceGUI(RESOURCE_TYPE _type);
    ~CResourceGUI();
};

