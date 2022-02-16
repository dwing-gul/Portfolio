#pragma once
#include "CShaderGUI.h"

class CComputeShaderGUI :
    public CShaderGUI
{
public :
    virtual void update();

public :
    CComputeShaderGUI();
    ~CComputeShaderGUI();
};

