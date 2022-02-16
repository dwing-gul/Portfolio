#pragma once
#include <Engine/CScript.h>

class CMouseRay :
    public CScript
{
private:
    Ray     m_mouseRay;
    float   m_distance;

    float   m_pickingTime;

    CGameObject* m_targetObj;
    CGameObject* m_camObj;

public:
    virtual void init();
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    CGameObject* GetTarget() { return m_targetObj; }

    CGameObject* MousePicking();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CMouseRay)

public:
    CMouseRay();
    CMouseRay(const CMouseRay& _origin);
    ~CMouseRay();
};

