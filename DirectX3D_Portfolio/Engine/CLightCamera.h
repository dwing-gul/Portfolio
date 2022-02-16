#pragma once
#include "CCamera.h"

class CLightCamera :
    public CCamera
{
private :
    map<ULONG64, vector<tInstObj>> m_mapInstGroup_S;

public :
    virtual void finalUpdate();

    void SortShadowObject();
    void render_shadowdepth();

public :
    CLightCamera();
    ~CLightCamera();
};

