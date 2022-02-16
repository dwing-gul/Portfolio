#pragma once
#include "Res.h"

struct tShaderParam
{
    SHADER_PARAM type;
    wstring      name;
};

class Shader :
    public Res
{
protected :
    ComPtr<ID3DBlob>     m_ErrorBlob;
    vector<tShaderParam> m_vectorParam;

public :
    void AddShaderParam(SHADER_PARAM _type, wstring _name) { m_vectorParam.push_back(tShaderParam{ _type, _name }); }
    const vector<tShaderParam>& GetShaderParam() { return m_vectorParam; }
    virtual void UpdateData() = 0;

    CLONE_DISABLE(Shader)

public :
    Shader(RESOURCE_TYPE _type);
    virtual ~Shader();
};

