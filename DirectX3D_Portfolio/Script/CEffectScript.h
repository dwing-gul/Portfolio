#pragma once
#include <Engine\CScript.h>

class CEffectScript :
    public CScript
{
private:
    Ptr<CMaterial>      m_sharedMaterial;
    Ptr<CMaterial>      m_cloneMaterial;
    
    CGameObject*        m_mainCam;
    CGameObject*        m_targetObj;

    Vec3                m_effectColor;
    Vec3                m_startColor;

    float               m_ratio;
    int                 m_mode;
    bool                m_bDecal;
    bool                m_bBuffEffect;
    bool                m_bAnimEffect;
    bool                m_bAlphaOff;

    virtual void start();
    virtual void update();
    virtual void lateUpdate();

public:
    void SetEffectColor(Vec3 _color);
    Vec3 GetEffectColor() { return m_effectColor; }

    void SetDecalEffect(bool _bDecal);
    bool GetDecal() { return m_bDecal; }

    void SetBuffEffectType();

    void SetMode(int _mode) { m_mode = _mode; }
    int GetMode() { return m_mode; }

    void SetRatio(float _Ratio) { m_ratio = _Ratio; }
    float GetRatio() { return m_ratio; }

    void SetAlphaOff() { m_bAlphaOff = true; }
    void Reset();

    void SetAnimMode();

    void SetCamObj(CGameObject* camObj) { m_mainCam = camObj; }
    void SetTargetObj(CGameObject* target) { m_targetObj = target; }

    void SetMaterialSetting();
    CMaterial* GetCurMaterial() { return m_cloneMaterial.Get(); }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CEffectScript)

public:
    CEffectScript();
    ~CEffectScript();
};