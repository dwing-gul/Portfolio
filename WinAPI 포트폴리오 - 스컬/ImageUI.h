#pragma once
#include "UI.h"

class ImageUI :
	public UI
{
private :
	static bool g_bCreateCollider;
	static bool g_bTileBoundary; // 타일의 경계를 설정하기 위한 변수
	static RENDER_MODE g_renderMode;
	static TILE_TYPE g_tileType;

private :
	Vector2 m_LT;
	int m_tileIdx;
	bool m_bTile;

public :
	virtual void update();
	virtual void render(HDC _dc);

	void SetLT(Vector2 _LT) { m_LT = _LT; }
	void SetTileIdx(int _index) { m_tileIdx = _index; }
	void SetIsTile(bool _tile) { m_bTile = _tile; }

	int GetTileIdx() { return m_tileIdx; }
	Vector2 GetLT() { return m_LT; }
	bool IsBoundary() { return g_bTileBoundary; }

	virtual void LButtonClick(Vector2 _mousePos);
	void LoadTexture();
	void Load(const wstring& _filePath);

	CLONE(ImageUI)

private :
	void SelectTile(int _imgIdx);

public :
	ImageUI();
	virtual ~ImageUI();

	friend class Scene_Tool;
};

