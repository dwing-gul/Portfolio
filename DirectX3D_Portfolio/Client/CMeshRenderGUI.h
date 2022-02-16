#pragma once
#include "CComponentGUI.h"
class CMeshRenderGUI :
    public CComponentGUI
{

private:
    int m_selectIndex;
    SHADER_PARAM m_selectedTextureParam;
public:
    virtual void update();

private :
    void SetMesh(DWORD_PTR _data1, DWORD_PTR _data2);
    void SetMaterial(DWORD_PTR _data1, DWORD_PTR _data2);

public:
    CMeshRenderGUI();
    virtual ~CMeshRenderGUI();
};

