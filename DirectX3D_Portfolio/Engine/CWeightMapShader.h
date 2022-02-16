#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;
class CTexture;

class CWeightMapShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_weightMap;
    CStructuredBuffer* m_raycastInfo;
    CStructuredBuffer* m_inputIndex;

    Ptr<CTexture> m_brushArrTex;
    Vec2 m_brushScale;
    int m_brushIndex;
    int m_weightIndex;

    int m_width;
    int m_height;

public:
    void SetWeightMap(CStructuredBuffer* _weightMap, UINT _width, UINT _height) { m_weightMap = _weightMap; m_width = _width; m_height = _height; }
    void SetInputBuffer(CStructuredBuffer* _raycastData) { m_raycastInfo = _raycastData; }
    void SetInputIndexBuffer(CStructuredBuffer* _input) { m_inputIndex = _input; }
    void SetBrushArrTex(Ptr<CTexture> _brushTex) { m_brushArrTex = _brushTex; }
    void SetBrushScale(Vec2 _scale) { m_brushScale = _scale; }
    void SetBrushIndex(int _index) { m_brushIndex = _index; }
    void SetWeightIndex(int _index) { m_weightIndex = _index; }

    virtual void UpdateData();
    virtual void Clear();
    virtual void Excute();

public:
    CWeightMapShader();
    ~CWeightMapShader();
};
