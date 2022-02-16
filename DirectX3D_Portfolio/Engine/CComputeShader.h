#pragma once
#include "CShader.h"

class CConstBuffer;
class CStructuredBuffer;

class CComputeShader :
    public CShader
{
protected:
    static const CConstBuffer* g_materialBuffer;

    tMaterialParam m_info;

private:
    ComPtr<ID3DBlob>                 m_CSBlob;
    ComPtr<ID3D11ComputeShader>      m_CS;

    UINT                             m_threadNumX;
    UINT                             m_threadNumY;
    UINT                             m_threadNumZ;

public:
    void CreateComputeShader(const wstring& _relativePath, const string& _funcName);

    virtual void UpdateData() = 0;
    virtual void Clear() = 0;
    virtual void Excute() = 0;

    void Dispatch(UINT _x, UINT _y, UINT _z);
    UINT GetThreadX() { return m_threadNumX; }
    UINT GetThreadY() { return m_threadNumY; }
    UINT GetThreadZ() { return m_threadNumZ; }

    int Load(const wstring& _filePath) { return 0; }
    CLONE_DISABLE(CComputeShader)

public:
    CComputeShader(UINT _x, UINT _y, UINT _z);
    ~CComputeShader();
};


