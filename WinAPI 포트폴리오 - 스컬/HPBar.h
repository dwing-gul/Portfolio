#pragma once
#include "UI.h"
class HPBar :
    public UI
{
private :
    Obj* m_target;

public :
    void SetTarget(Obj* _target) { m_target = _target; }

    virtual void update();
    virtual void render(HDC _dc);
    CLONE(HPBar);

public:
    HPBar();
    virtual ~HPBar();
};

