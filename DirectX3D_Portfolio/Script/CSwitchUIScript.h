#pragma once
#include "CUIScript.h"
class CSwitchUIScript :
    public CUIScript
{
private :
    static UINT g_nextSwitchID;

private :
    UINT m_switchID;
    CGameObject* m_cam;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CSwitchUIScript)

public:
    CSwitchUIScript();
    ~CSwitchUIScript();
};

