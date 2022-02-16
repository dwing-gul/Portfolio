#pragma once
#include "ComponentGUI.h"

class TransformGUI :
    public ComponentGUI
{
public :
    virtual void update();

public :
    TransformGUI();
    ~TransformGUI();
};

