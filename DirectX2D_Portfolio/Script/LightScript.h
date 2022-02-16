#pragma once
#include <Engine/Script.h>

class LightScript :
    public Script
{
private :
    float m_maxIntensity;
    float m_curIntensity;
    float m_lightRatio;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void AddLight(float _intensity)
    {
        m_curIntensity += _intensity;
        
        if (m_curIntensity > m_maxIntensity)
        {
            m_curIntensity = m_maxIntensity;
        }
    }

    void SubLight(float _intensity)
    {
        m_curIntensity -= _intensity;

        if (m_curIntensity < 0.f)
        {
            m_curIntensity = 0.f;
        }
    }

    float GetLightRatio() { return m_lightRatio; }
    float GetCurIntensity() { return m_curIntensity; }

    CLONE(LightScript)

public:
    LightScript();
    ~LightScript();
};

