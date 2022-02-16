#pragma once
#include "CComponent.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CMesh.h"

class CStructuredBuffer;

class CTileMap :
    public CComponent
{
private:
    Ptr<CMesh> m_mesh;
    Ptr<CMaterial> m_material;
    Ptr<CTexture> m_atlas;
    POINT m_tileMapSize;
    vector<tTileInfo> m_vecTileInfo;

    CStructuredBuffer* m_tileMapBuffer;

public:
    void SetTileAtlas(Ptr<CTexture> _texture);
    void AddTileInfo(tTileInfo _info) { m_vecTileInfo.push_back(_info); }
    void SetTileMapSize(POINT _size) { m_tileMapSize = _size; }
    void SetVectorTileInfo(vector<tTileInfo> _vector) { m_vecTileInfo.clear(); m_vecTileInfo = _vector; }

    POINT GetTileMapSize() { return m_tileMapSize; }
    Ptr<CTexture> GetTileAtlas() { return m_atlas; }
    vector<tTileInfo>& GetTileInfo() { return m_vecTileInfo; }

    virtual void finalUpdate();
    void render();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CTileMap);

public:
    CTileMap();
    ~CTileMap();
};
