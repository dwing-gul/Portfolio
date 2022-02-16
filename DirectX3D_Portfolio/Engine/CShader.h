#pragma once
#include "CRes.h"

struct tShaderParam
{
    SHADER_PARAM type;
    wstring name;
};

class CShader :
    public CRes
{
protected:
    ComPtr<ID3DBlob>     m_ErrorBlob;
    vector<tShaderParam> m_vecParam;

public:
    void AddShaderParam(SHADER_PARAM _type, wstring _name) { m_vecParam.push_back(tShaderParam{ _type, _name }); }
    const vector<tShaderParam>& GetShaderParam() { return m_vecParam; }

    virtual void UpdateData() = 0;
    CLONE_DISABLE(CShader);

public:
    CShader(RESOURCE_TYPE _type);
    virtual ~CShader();
};

