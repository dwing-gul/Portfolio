#pragma once
#include "Obj.h"

class Tile :
    public Obj
{
private :
    static int g_imgIdx;

private :
    int m_imgIdx;
    RENDER_MODE m_renderMode;
    TILE_TYPE m_tileType; // 타일의 타입

public :
    virtual void update();
    virtual void render(HDC _dc);

    void SetImgIdx(int _idx) { m_imgIdx = _idx; }
    void SetRenderMode(RENDER_MODE _mode) { m_renderMode = _mode; }
    void SetTileType(TILE_TYPE _type) { m_tileType = _type; }

    int GetImgIdx() { return m_imgIdx; }
    TILE_TYPE GetTileType() { return m_tileType; }

    virtual void Save(FILE* _file);
    virtual void Load(FILE* _file);

    CLONE(Tile)

public :
    Tile();
    virtual ~Tile();

    friend class Scene_Tool;
    friend class EventManager;
    friend class UI;
};

