#pragma once
#include "CComponentGUI.h"

class CParticleSystemGUI :
    public CComponentGUI
{
public :
    virtual void update();

private :
    void SetTexture(DWORD_PTR _data1, DWORD_PTR _data2);
    void GetTextureName(vector<wstring>& _vectorName);

public :
    CParticleSystemGUI();
    ~CParticleSystemGUI();
};

