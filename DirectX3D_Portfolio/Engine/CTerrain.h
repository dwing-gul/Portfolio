#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"

struct tRaycastOut
{
    Vec2 UV;
    int dist;
    int bSuccess;
};

struct tWeight_4
{
    float arrWeight[4];
};

struct tWeight_8
{
    float arrWeight[8];
};

class CStructuredBuffer;

class CTerrain :
    public CComponent
{
private:
    Ptr<CMesh> m_mesh;
    Ptr<CMaterial> m_material;

    Ptr<CTexture> m_heightMap;
    CStructuredBuffer* m_weightMap;

    Ptr<CRaycastShader> m_CSRaycast;
    Ptr<CHeightMapShader> m_CSHeight;
    Ptr<CWeightMapShader> m_CSWeight;
    Ptr<CTexture> m_brushArrTex;
    Ptr<CTexture> m_tileArrTex;

    CStructuredBuffer* m_crossBuffer;
    CStructuredBuffer* m_arrIndexBuffer;

    Vec2 m_brushScale;
    UINT m_brushIndex;

    UINT m_faceX;
    UINT m_faceZ;

    float m_maxTessLevel;
    Vec2 m_tessDistMinMax;

    UINT m_weightWidth;
    UINT m_weightHeight;
    UINT m_weightIndex;

    TERRAIN_MODE m_mode;

    bool m_bWireFrame;

public:
    virtual void finalUpdate();
    void render();

    UINT* GetFaceCount() { UINT face[2] = { m_faceX, m_faceZ }; return face; }
    float GetMaxTessLevel() { return m_maxTessLevel; }
    UINT GetWeightIndex() { return m_weightIndex; }
    UINT GetBrushIndex() { return m_brushIndex; }
    TERRAIN_MODE GetTerrainMode() { return m_mode; }
    Vec2 GetBrushScale() { return m_brushScale; }
    Vec2 GetTessDistMinMax() { return m_tessDistMinMax; }
    Ptr<CTexture> GetBrushArrTex() { return m_brushArrTex; }
    Ptr<CTexture> GetTileArrTex() { return m_tileArrTex; }
    ULONG64 GetInstID();
    bool IsWireFrame() { return m_bWireFrame; }

    void SetFaceCount(UINT _faceX, UINT _faceZ);
    void SetMaxTessLevel(float _level) { m_maxTessLevel = _level; }
    void SetWeightIndex(UINT _index) { m_weightIndex = _index; }
    void SetBrushIndex(UINT _index) { m_brushIndex = _index; }
    void SetTerrainMode(TERRAIN_MODE _mode) { m_mode = _mode; }
    void SetBrushScale(Vec2 _scale) { m_brushScale = _scale; }
    void SetTessDistMinMax(Vec2 _minmax) { m_tessDistMinMax = _minmax; }
    void SetBrushArrTex(Ptr<CTexture> _brush) { m_brushArrTex = _brush; }
    void SetTileArrTex(Ptr<CTexture> _tile) { m_tileArrTex = _tile; }
    void SetWireFrame(bool _wireframe) { m_bWireFrame = _wireframe; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CTerrain);

private:
    void CreateTerrainMesh();

public:
    CTerrain();
    ~CTerrain();
};

