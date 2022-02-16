#pragma once
#include "ComponentGUI.h"

class TileMapGUI :
    public ComponentGUI
{
public :
    virtual void update();

private :
    void SetAtlas(DWORD_PTR _data1, DWORD_PTR _data2);

public :
    TileMapGUI();
    ~TileMapGUI();
};

