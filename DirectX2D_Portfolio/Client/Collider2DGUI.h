#pragma once
#include "ComponentGUI.h"

class Collider2DGUI :
    public ComponentGUI
{
public :
    virtual void update();

public :
    Collider2DGUI();
    ~Collider2DGUI();
};

