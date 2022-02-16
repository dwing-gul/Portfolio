#pragma once
#include <Engine/CScript.h>

class CCameraScript :
    public CScript
{
private :
    bool m_bChange;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    void Change() { m_bChange = true; }

    bool IsChange() { return m_bChange; }

    CLONE(CCameraScript)

public:
    CCameraScript();
    ~CCameraScript();
};

