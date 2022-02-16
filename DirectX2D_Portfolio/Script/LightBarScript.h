#pragma once
#include "UIScript.h"

class LightBarScript :
	public UIScript
{
private :
    float m_originXScaleRate;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(LightBarScript)

public:
    LightBarScript();
    ~LightBarScript();
};

