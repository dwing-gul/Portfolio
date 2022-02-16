#pragma once
#include "InspectorInfoGUI.h"
#include <Engine/GameObject.h>

class ComponentGUI :
    public InspectorInfoGUI
{
private :
    GameObject*     m_target;
    COMPONENT_TYPE  m_type;
    bool            m_bSubComponent;

public :
    void SetTarget(GameObject* _target) { m_target = _target; }
    GameObject* GetTarget() { return m_target; }

    virtual void update() = 0;

protected :
    virtual bool Start();
    virtual void End();

private :
    void SubComponent();

public :
    ComponentGUI(COMPONENT_TYPE _type);
    virtual ~ComponentGUI();
};

