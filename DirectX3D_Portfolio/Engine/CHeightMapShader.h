#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CHeightMapShader :
    public CComputeShader
{
private:
    Ptr<CTexture> m_heightMap;
    Ptr<CTexture> m_brushTex;

    CStructuredBuffer* m_input;
    CStructuredBuffer* m_inputIndex;
    Vec2 m_scale;
    int m_brushIndex;
    int m_mode;

public:
    void SetHeightMap(Ptr<CTexture> _tex) { m_heightMap = _tex; }
    void SetBrushTexture(Ptr<CTexture> _tex) { m_brushTex = _tex; }
    void SetBrushScale(Vec2 _scale) { m_scale = _scale; }
    void SetBrushIndex(int _index) { m_brushIndex = _index; }
    void SetInputBuffer(CStructuredBuffer* _input) { m_input = _input; }
    void SetInputIndexBuffer(CStructuredBuffer* _input) { m_inputIndex = _input; }
    void SetHeightMapMode(int _mode) { m_mode = _mode; }

    virtual void UpdateData();
    virtual void Clear();
    virtual void Excute();

public:
    CHeightMapShader();
    ~CHeightMapShader();
};

