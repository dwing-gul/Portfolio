#pragma once
#include "ComponentGUI.h"

class MeshRenderGUI :
    public ComponentGUI
{
public :
    virtual void update();

private :
    void SetMesh(DWORD_PTR _data1, DWORD_PTR _data2);
    void SetMaterial(DWORD_PTR _data1, DWORD_PTR _data2);

public :
    MeshRenderGUI();
    ~MeshRenderGUI();
};

