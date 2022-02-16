#pragma once
#include "ShaderGUI.h"

class ComputeShaderGUI :
    public ShaderGUI
{
public :
    virtual void update();

public :
    ComputeShaderGUI();
    ~ComputeShaderGUI();
};

