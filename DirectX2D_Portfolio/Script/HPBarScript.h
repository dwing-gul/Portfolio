#pragma once
#include "UIScript.h"
#include <Engine/ResourceManager.h>

class HPBarScript :
    public UIScript
{
private :
    GameObject*     m_owner;
    Ptr<Material>   m_sharedMaterial;
    Ptr<Material>   m_cloneMaterial;
    bool            m_bBase;
    bool            m_bStress; // 스트레스 바인가?
    bool            m_bHit; // 체력이 서서히 주는 것을 표현하기위한 값
    bool            m_bHeal;
    bool            m_bFirst;
    float           m_length; // hp바 전체 길이
    float           m_resultLength; // 최종 길이
    int             m_stress;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetBase(bool _base) { m_bBase = _base; }
    void SetStress(bool _stress) { m_bStress = _stress; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(HPBarScript)

public:
    HPBarScript();
    ~HPBarScript();
};

