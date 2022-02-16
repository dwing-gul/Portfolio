#pragma once
#include "ImageUI.h"
class Background :
    public ImageUI
{
private :
    RENDER_MODE m_renderMode;
    POINT m_magnification;

public :
    void SetRenderMode(RENDER_MODE _mode) { m_renderMode = _mode; }
    void SetMagnification(POINT _pt) { m_magnification = _pt; }

    virtual void update();
    virtual void render(HDC _dc);

    virtual void Save(FILE* _file);
    virtual void Load(FILE* _file);

    CLONE(Background);

public :
    Background();
    ~Background();
};

