#pragma once
#include "Obj.h"
class Effect :
    public Obj
{
private :
    Vector2 m_magnification;

public :
    void SetMagnification(Vector2 _magnification) { m_magnification = _magnification; }

    Vector2 GetMagnification() { return m_magnification; }

    virtual void update();
    virtual void render(HDC _dc);

    CLONE(Effect)

public :
    Effect();
    ~Effect();
};

