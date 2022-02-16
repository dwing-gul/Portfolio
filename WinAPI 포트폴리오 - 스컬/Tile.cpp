#include "stdafx.h"
#include "Tile.h"
#include "Camera.h"
#include "Core.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Collider.h"
#include "Scene.h"

int Tile::g_imgIdx = -1;

Tile::Tile() :
	m_imgIdx(-1),
	m_renderMode(RENDER_MODE::TRANSPARENTBLT),
	m_tileType(TILE_TYPE::NONE)
{
	CreateTexture(L"TileSet2", L"Texture\\TileSet2.bmp");
}

Tile::~Tile()
{
	int a = 0;
}

void Tile::update()
{
}

void Tile::render(HDC _dc)
{
	Vector2 renderPos = Camera::GetInst()->GetRenderPos(GetPos());
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if (-1 == m_imgIdx)
	{
		if (SCENE_TYPE::TOOL == curScene->GetType())
		{
			HBRUSH hollowBrush = Core::GetInst()->GetBrush(BRUSH_TYPE::HOLLOW);
			HBRUSH prevBrush = (HBRUSH)SelectObject(_dc, hollowBrush);
			Rectangle(_dc, (int)renderPos.x, (int)renderPos.y, (int)renderPos.x + TILE_SIZE, (int)renderPos.y + TILE_SIZE);
			SelectObject(_dc, prevBrush);
		}
	}
	else
	{
		Texture* tileTex = GetTexture();
		
		int maxCol = tileTex->GetHeight() / TILE_SIZE;
		int maxRow = tileTex->GetWidth() / TILE_SIZE;

		int col = m_imgIdx / maxRow;
		int row = m_imgIdx % maxRow;

		if (maxCol <= col)
		{
			assert(nullptr);
			return;
		}

		if (RENDER_MODE::ALPHABLEND == m_renderMode)
		{
			BLENDFUNCTION blend = {};
			blend.AlphaFormat = AC_SRC_ALPHA;
			blend.BlendFlags = 0;
			blend.BlendOp = AC_SRC_OVER;
			blend.SourceConstantAlpha = 255;

			AlphaBlend(_dc, (int)renderPos.x, (int)renderPos.y, TILE_SIZE, TILE_SIZE,
				tileTex->GetTextureDC(), row * TILE_SIZE, col * TILE_SIZE, TILE_SIZE, TILE_SIZE, blend);
		}
		else if (RENDER_MODE::BITBLT == m_renderMode)
		{
			BitBlt(_dc, (int)renderPos.x, (int)renderPos.y, TILE_SIZE, TILE_SIZE,
				tileTex->GetTextureDC(), row * TILE_SIZE, col * TILE_SIZE, SRCCOPY);
		}
	}

	Obj::render(_dc);
}

void Tile::Save(FILE* _file)
{
	fwrite(&m_imgIdx, sizeof(int), 1, _file);
	fwrite(&m_renderMode, sizeof(RENDER_MODE), 1, _file);
	fwrite(&m_tileType, sizeof(TILE_TYPE), 1, _file);
	bool hasCollider = false;
	if (GetCollider() != nullptr)
	{
		hasCollider = true;
		fwrite(&hasCollider, sizeof(bool), 1, _file);
		Vector2 offsetPos = GetCollider()->GetFinalPos() - GetPos();
		Vector2 scale = GetCollider()->GetScale();
		fwrite(&offsetPos, sizeof(Vector2), 2, _file);
		fwrite(&scale, sizeof(Vector2), 2, _file);
	}
	else
	{
		fwrite(&hasCollider, sizeof(bool), 1, _file);
	}
}

void Tile::Load(FILE* _file)
{
	fread(&m_imgIdx, sizeof(int), 1, _file);
	fread(&m_renderMode, sizeof(RENDER_MODE), 1, _file);
	fread(&m_tileType, sizeof(TILE_TYPE), 1, _file);
	bool hasCollider = false;
	fread(&hasCollider, sizeof(bool), 1, _file);
	if (hasCollider)
	{
		CreateCollider();
		Vector2 offsetPos = Vector2(0, 0);
		Vector2 scale = Vector2(0, 0);
		fread(&offsetPos, sizeof(Vector2), 2, _file);
		fread(&scale, sizeof(Vector2), 2, _file);
		GetCollider()->SetOffsetPos(offsetPos);
		GetCollider()->SetScale(scale);
	}
}

