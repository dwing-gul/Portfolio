#include "pch.h"
#include "TileMap.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "Transform.h"
#include "StructuredBuffer.h"

TileMap::TileMap() :
	Component(COMPONENT_TYPE::TILEMAP),
	m_tileMapSize(POINT{ 0, 0 }),
	m_tileMapBuffer(nullptr)
{
	m_mesh = ResourceManager::GetInst()->FindRes<Mesh>(L"RectMesh");
	m_material = ResourceManager::GetInst()->FindRes<Material>(L"TileMapMaterial");

	m_tileMapBuffer = new StructuredBuffer;
	m_tileMapBuffer->Create(STRUCTURED_TYPE::READONLY, sizeof(tTileInfo), 5);
}

TileMap::~TileMap()
{
	Safe_Delete(m_tileMapBuffer);
}

void TileMap::finalUpdate()
{
	int totalSize = m_tileMapSize.x * m_tileMapSize.y;

	if ((int)m_vectorTileInfo.size() < totalSize)
	{
		for (int i = 0; i < totalSize - (int)m_vectorTileInfo.size(); ++i)
		{
			AddTileInfo(tTileInfo{ Vector2(0.f, 0.f), Vector2(1.f / 8.f, 1.f / 6.f) });
		}
	}

	if (m_tileMapBuffer->GetElementCount() < (UINT)m_vectorTileInfo.size())
	{
		m_tileMapBuffer->Create(STRUCTURED_TYPE::READONLY, m_tileMapBuffer->GetElementSize(), m_tileMapBuffer->GetElementCount() * 2);
	}

	m_tileMapBuffer->SetData(m_vectorTileInfo.data(), (UINT)m_vectorTileInfo.size());
	m_tileMapBuffer->UpdateData(52);
}

void TileMap::render()
{
	m_material->SetData(SHADER_PARAM::INT_0, &m_tileMapSize.x);
	m_material->SetData(SHADER_PARAM::INT_1, &m_tileMapSize.y);

	GetTransform()->UpdateData();

	m_mesh->UpdateData();
	m_material->UpdateData();

	m_mesh->render();

	m_material->Clear();
}

void TileMap::SetTileAtlas(Ptr<Texture> _texture)
{
	m_atlasTexture = _texture;
	m_material->SetData(SHADER_PARAM::TEXTURE_0, m_atlasTexture.Get());
}

void TileMap::SaveToScene(FILE* _file)
{
	Component::SaveToScene(_file);
	SaveResourceToFile(m_atlasTexture, _file);
	SaveToFile(&m_tileMapSize, _file);

	UINT tileCount = (UINT)m_vectorTileInfo.size();
	SaveToFile(&tileCount, _file);

	for (size_t i = 0; i < m_vectorTileInfo.size(); ++i)
	{
		SaveToFile(&m_vectorTileInfo[i], _file);
	}
}

void TileMap::LoadFromScene(FILE* _file)
{
	Component::LoadFromScene(_file);
	LoadResourceFromFile(m_atlasTexture, _file);
	m_material->SetData(SHADER_PARAM::TEXTURE_0, m_atlasTexture.Get());
	LoadFromFile(&m_tileMapSize, _file);

	UINT tileCount = 0;
	LoadFromFile(&tileCount, _file);

	for (UINT i = 0; i < tileCount; ++i)
	{
		tTileInfo tile = {};
		LoadFromFile(&tile, _file);
		m_vectorTileInfo.push_back(tile);
	}
}
