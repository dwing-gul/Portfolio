#include "pch.h"
#include "CTileMap.h"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"

CTileMap::CTileMap() :
	CComponent(COMPONENT_TYPE::TILEMAP),
	m_tileMapBuffer(nullptr)
{
	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"TileMapMaterial");

	m_tileMapBuffer = new CStructuredBuffer;
	m_tileMapBuffer->Create(STRUCTURED_TYPE::READONLY, sizeof(tTileInfo), 5);
}

CTileMap::~CTileMap()
{
	Safe_Delete_Ptr(m_tileMapBuffer);
}

void CTileMap::finalUpdate()
{
	int totalSize = m_tileMapSize.x * m_tileMapSize.y;

	if (m_vecTileInfo.size() < totalSize)
	{
		for (int i = 0; i < totalSize - (int)m_vecTileInfo.size(); ++i)
		{
			AddTileInfo(tTileInfo{ Vec2(0.f, 0.f), Vec2(1.f / 8.f, 1.f / 6.f) });
		}
	}

	if (m_tileMapBuffer->GetElementCount() < m_vecTileInfo.size())
	{
		m_tileMapBuffer->Create(STRUCTURED_TYPE::READONLY, m_tileMapBuffer->GetElementSize(), m_tileMapBuffer->GetElementCount() * 2);
	}

	m_tileMapBuffer->SetData(m_vecTileInfo.data(), (UINT)m_vecTileInfo.size());
	m_tileMapBuffer->UpdateData(53);
}

void CTileMap::render()
{
	m_material->SetData(SHADER_PARAM::INT_0, &m_tileMapSize.x);
	m_material->SetData(SHADER_PARAM::INT_1, &m_tileMapSize.y);

	Transform()->UpdateData();

	m_mesh->UpdateData(0);
	m_material->UpdateData();

	m_mesh->render(0);

	m_material->Clear();
}

void CTileMap::SetTileAtlas(Ptr<CTexture> _texture)
{
	m_atlas = _texture;
	m_material->SetData(SHADER_PARAM::TEXTURE_0, _texture.Get());
}

void CTileMap::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	SaveResourceToFile(m_atlas, _file);
	SaveToFile(&m_tileMapSize, _file);

	UINT tileCount = (UINT)m_vecTileInfo.size();
	SaveToFile(&tileCount, _file);

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		SaveToFile(&m_vecTileInfo[i], _file);
	}
}

void CTileMap::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	LoadResourceFromFile(m_atlas, _file);
	m_material->SetData(SHADER_PARAM::TEXTURE_0, m_atlas.Get());
	LoadFromFile(&m_tileMapSize, _file);

	UINT tileCount = 0;
	LoadFromFile(&tileCount, _file);

	for (UINT i = 0; i < tileCount; ++i)
	{
		tTileInfo tile = {};
		LoadFromFile(&tile, _file);
		m_vecTileInfo.push_back(tile);
	}
}
