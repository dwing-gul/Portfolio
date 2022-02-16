#pragma once
#include "CComputeShader.h"

class CCopyTextureShader :
    public CComputeShader
{
private:
    Ptr<CTexture> m_srcTex;
    Ptr<CTexture> m_destTex;

public:
    void SetSrcTexture(Ptr<CTexture> _texture);
    void SetDestTexture(Ptr<CTexture> _texture);
    void IsSampling(bool _use) { m_info.intArr[0] = (UINT)_use; }

public:
    virtual void UpdateData();
    virtual void Clear();
    virtual void Excute();

public:
    CCopyTextureShader();
    ~CCopyTextureShader();
};


