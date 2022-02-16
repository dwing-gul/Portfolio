#pragma once
#include "CInspectorInfoGUI.h"
#include <Engine/CGameObject.h>

class CComponentGUI :
    public CInspectorInfoGUI
{
private :
    CGameObject* m_target;
    COMPONENT_TYPE m_componentType;
    bool            m_bSubComponent;

public :
    void SetTarget(CGameObject* _target) { m_target = _target; }
    CGameObject* GetTarget() { return m_target; }

    virtual void update() = 0;

protected :
    virtual bool Start();
    virtual void End();

private:
    void SubComponent();

public :
    CComponentGUI(COMPONENT_TYPE _type);
    virtual ~CComponentGUI();
};

