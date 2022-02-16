#pragma once
#include "CComponentGUI.h"

class CTileMapGUI :
    public CComponentGUI
{
public :
    virtual void update();

private :
    void SetAtlas(DWORD_PTR _data1, DWORD_PTR _data2);

public :
    CTileMapGUI();
    ~CTileMapGUI();
};

