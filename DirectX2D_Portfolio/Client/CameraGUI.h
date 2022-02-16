#pragma once
#include "ComponentGUI.h"

class CameraGUI :
    public ComponentGUI
{
public :
    virtual void update();

public :
    CameraGUI();
    ~CameraGUI();
};

