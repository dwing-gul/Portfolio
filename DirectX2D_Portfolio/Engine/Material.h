#pragma once
#include "Res.h"
#include "GraphicsShader.h"
#include "Texture.h"

class Material :
    public Res
{
private :
    Ptr<GraphicsShader>  m_shader;
    tMaterialParam       m_param;
    Ptr<Texture>         m_arrTex[(UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0];

public :
    void SetShader(Ptr<GraphicsShader> _shader) { m_shader = _shader; }
    void SetData(SHADER_PARAM _param, void* _data);

    Ptr<GraphicsShader> GetShader() { return m_shader; }
    void GetData(SHADER_PARAM _param, void* _data);

    virtual void UpdateData();
    virtual void Save(const wstring& _relativePath);
    virtual int Load(const wstring& _filePath);
    static void Clear();

    virtual Material* Clone();

public :
    Material();
    Material(bool _bDefault);
    ~Material();
};

