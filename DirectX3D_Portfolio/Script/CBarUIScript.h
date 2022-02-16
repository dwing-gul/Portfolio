#pragma once
#include "CUIScript.h"

class CBarUIScript :
    public CUIScript
{
private :
    float m_ratio;
    BAR_MODE m_barMode;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetBarMode(BAR_MODE _mode) { m_barMode = _mode; }
    void SetRatio(float _ratio) { m_ratio = _ratio; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CBarUIScript)

public:
    CBarUIScript();
    ~CBarUIScript();
};

