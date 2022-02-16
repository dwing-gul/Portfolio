#pragma once
#include "CResourceGUI.h"

class CMaterialGUI :
    public CResourceGUI
{
private:
    SHADER_PARAM m_selectedTextureParam;

public:
    virtual void update();

public :
    void SetShader(DWORD_PTR _shaderName, DWORD_PTR _data);
    void SelectTexture(DWORD_PTR _textureName, DWORD_PTR _data);

public:
    CMaterialGUI();
    ~CMaterialGUI();
};

