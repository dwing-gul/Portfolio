#pragma once
#include <Engine/Script.h>

enum class WALL_TYPE
{
    RIGHT,
    LEFT
};

class WallScript :
    public Script
{
private :
    WALL_TYPE m_wallType;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetWallType(WALL_TYPE _type) { m_wallType = _type; }
    WALL_TYPE GetWallType() { return m_wallType; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(WallScript)

public:
    WallScript();
    ~WallScript();
};

