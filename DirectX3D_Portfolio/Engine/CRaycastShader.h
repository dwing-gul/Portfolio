#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;
class CTexture;

class CRaycastShader :
    public CComputeShader
{
private:
    Ptr<CTexture> m_heightMap;

    UINT m_xFace;
    UINT m_zFace;

    tRay m_ray;

    CStructuredBuffer* m_output;
    CStructuredBuffer* m_index;

public:
    void SetFaceCount(UINT _x, UINT _z) { m_xFace = _x, m_zFace = _z; }
    void SetCameraRay(const tRay& _ray) { m_ray = _ray; }
    void SetOutputBuffer(CStructuredBuffer* _outputBuffer) { m_output = _outputBuffer; }
    void SetOutputIndexBuffer(CStructuredBuffer* _outputBuffer) { m_index = _outputBuffer; }
    void SetHeightMap(Ptr<CTexture> _heightMap) { m_heightMap = _heightMap; }

    virtual void UpdateData();
    virtual void Clear();
    virtual void Excute();

public:
    CRaycastShader();
    ~CRaycastShader();
};

