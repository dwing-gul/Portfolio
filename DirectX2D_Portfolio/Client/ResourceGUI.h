#pragma once
#include "InspectorInfoGUI.h"
class ResourceGUI :
    public InspectorInfoGUI
{
private:
    Res*            m_target;
    RESOURCE_TYPE   m_type;

public:
    void SetTarget(Res* _target) { m_target = _target; }
    Res* GetTarget() { return m_target; }

    virtual void update() = 0;

protected:
    virtual bool Start();
    virtual void End();

public:
    ResourceGUI(RESOURCE_TYPE _type);
    virtual ~ResourceGUI();
};

