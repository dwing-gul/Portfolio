#pragma once

class Obj;
class Player;

class Scene
{
private :
	vector<Obj*> m_arrObj[(UINT)GROUP_TYPE::END];
	vector<bool> m_vectorTileUpdate;
	SCENE_TYPE m_type;

	UINT m_col;
	UINT m_row;

public :
	void AddObj(Obj* _obj, GROUP_TYPE _eType);
	void SetType(SCENE_TYPE _eType) { m_type = _eType; }
	void InitTileUpdate();

	SCENE_TYPE GetType() { return m_type; }
	vector<Obj*>& GetObjects(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	UINT GetRow() { return m_row; }
	UINT GetCol() { return m_col; }

	void DeleteObj(GROUP_TYPE _eType);
	void DeleteOrder();
	void DeleteAll();

	virtual void update();
	virtual void lateUpdate();
	virtual void render(HDC _dc);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	void CreateTile(UINT _col, UINT _row);
	void PlayerUISetting(Player* _player);
	void UIChange(Player* _player);
	void ArrClear();

private :
	void update_tile();
	void lateUpdate_tile();
	void render_tile(HDC _dc);

protected :
	void LoadMap(const wstring& _relativePath);
	void LoadMonster(const wstring& _relativePath);

public :
	Scene();
	virtual ~Scene();
};

