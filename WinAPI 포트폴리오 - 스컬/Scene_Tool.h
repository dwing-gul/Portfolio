#pragma once
#include "Scene.h"

class Background;

class Scene_Tool :
    public Scene
{
private :
    HMENU m_menu;

public :
    virtual void update();

    virtual void Enter();
    virtual void Exit();

    void SaveMapSelect();
    void LoadMapSelect();

    void SaveMonsterSelect();
    void LoadMonsterSelect();

    void TextureLoad();
    void TileLoad(int _col, int _row);
    void CreateObjUI();
    void CreateBackground(const wstring& _name, RENDER_MODE _mode);
    void CreateMonster(const wstring& _name, Texture* _texture, MONSTER_GROUP _type);

private:
    void MouseCheck();
    void SaveMap(const wstring& _relativePath);
    void SaveMonster(const wstring& _relativePath);

public :
    Scene_Tool();
    virtual ~Scene_Tool();
};

