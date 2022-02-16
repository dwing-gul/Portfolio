#pragma once
#include "CComponentGUI.h"

class CTerrainGUI :
    public CComponentGUI
{
public:
    virtual void update();

private :
    void SetBrushTexture(DWORD_PTR _data1, DWORD_PTR _data2);
    void SetTileTexture(DWORD_PTR _data1, DWORD_PTR _data2);

public:
    CTerrainGUI();
    virtual ~CTerrainGUI();
};

