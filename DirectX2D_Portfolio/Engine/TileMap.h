#pragma once
#include "Component.h"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"

class StructuredBuffer;

class TileMap :
    public Component
{
private :
    Ptr<Mesh>         m_mesh;
    Ptr<Material>     m_material;
    Ptr<Texture>      m_atlasTexture;
    POINT             m_tileMapSize;
    vector<tTileInfo> m_vectorTileInfo;
    StructuredBuffer* m_tileMapBuffer;

public :
    void SetTileAtlas(Ptr<Texture> _texture);
    void AddTileInfo(tTileInfo _info) { m_vectorTileInfo.push_back(_info); }
    void SetTileMapSize(POINT _size) { m_tileMapSize = _size; }

    POINT GetTileMapSize() { return m_tileMapSize; }
    Ptr<Texture> GetTileAtlas() { return m_atlasTexture; }  
    vector<tTileInfo>& GetTileInfo() { return m_vectorTileInfo; }

    virtual void finalUpdate();
    void render();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(TileMap)

public :
    TileMap();
    ~TileMap();
};

