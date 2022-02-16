#include "pch.h"
#include "CTerrain.h"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CKeyManager.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CStructuredBuffer.h"

CTerrain::CTerrain() :
	CComponent(COMPONENT_TYPE::TERRAIN),
	m_brushScale(Vec2(0.2f, 0.2f)),
	m_brushIndex(0),
	m_faceX(0),
	m_faceZ(0),
	m_maxTessLevel(6.f),
	m_tessDistMinMax(Vec2(1000.f, 5000.f)),
	m_weightWidth(0),
	m_weightHeight(0),
	m_weightIndex(0),
	m_mode(TERRAIN_MODE::NONE),
	m_bWireFrame(false)
{
	SetFaceCount(64, 64);
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"TerrainMaterial");

	m_heightMap = CResourceManager::GetInst()->CreateTexture(L"HeightMap", 2048, 2048, DXGI_FORMAT_R32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	m_material->SetData(SHADER_PARAM::TEXTURE_0, m_heightMap.Get());

	m_CSRaycast = (CRaycastShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
	m_CSHeight = (CHeightMapShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();
	m_CSWeight = (CWeightMapShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"WeightMapShader").Get();

	m_brushArrTex = CResourceManager::GetInst()->FindRes<CTexture>(L"BRUSH_ARR");
	m_tileArrTex = CResourceManager::GetInst()->FindRes<CTexture>(L"TILE_ARR");

	m_crossBuffer = new CStructuredBuffer;
	m_crossBuffer->Create(STRUCTURED_TYPE::DUAL, sizeof(tRaycastOut), 1, nullptr);

	m_arrIndexBuffer = new CStructuredBuffer;
	m_arrIndexBuffer->Create(STRUCTURED_TYPE::DUAL, sizeof(int), 1, nullptr);

	m_weightWidth = 1024;
	m_weightHeight = 1024;

	m_weightMap = new CStructuredBuffer;
	m_weightMap->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(tWeight_4), m_weightWidth * m_weightHeight);
}

CTerrain::~CTerrain()
{
	Safe_Delete_Ptr(m_crossBuffer);
	Safe_Delete_Ptr(m_arrIndexBuffer);
	Safe_Delete_Ptr(m_weightMap);
}

void CTerrain::finalUpdate()
{
	if (KEY_TAP(KEY::N0))
	{
		m_mode = TERRAIN_MODE::NONE;
	}
	else if (KEY_TAP(KEY::N1))
	{
		m_mode = TERRAIN_MODE::HEIGHT_MAP_UP;
	}
	else if (KEY_TAP(KEY::N2))
	{
		m_mode = TERRAIN_MODE::HEIGHT_MAP_DOWN;
	}
	else if (KEY_TAP(KEY::N3))
	{
		m_mode = TERRAIN_MODE::HEIGHT_MAP_FLATTENING;
	}
	else if (KEY_TAP(KEY::N4))
	{
		m_mode = TERRAIN_MODE::SPLAT;
	}

	if (TERRAIN_MODE::NONE == m_mode)
	{
		return;
	}

	CCamera* mainCam = CRenderManager::GetInst()->GetMainCam();
	Vec3 mainCamPos = mainCam->Transform()->GetWorldPos();

	m_material->SetData(SHADER_PARAM::VEC4_0, &mainCamPos);

	const Matrix& worldMatrixInverse = Transform()->GetWorldMatrixInverse();
	const tRay& ray = mainCam->GetRay();

	tRay camRay = {};
	camRay.start = XMVector3TransformCoord(ray.start, worldMatrixInverse);
	camRay.dir = XMVector3TransformNormal(ray.dir, worldMatrixInverse);
	camRay.dir.Normalize();

	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_crossBuffer->SetData(&out, 1);

	int indexReset = 0;
	m_arrIndexBuffer->SetData(&indexReset, 1);

	m_CSRaycast->SetHeightMap(m_heightMap);
	m_CSRaycast->SetFaceCount(m_faceX, m_faceZ);
	m_CSRaycast->SetCameraRay(camRay);
	m_CSRaycast->SetOutputBuffer(m_crossBuffer);
	m_CSRaycast->SetOutputIndexBuffer(m_arrIndexBuffer);

	m_CSRaycast->Excute();

	if ((m_tileArrTex->GetArrSize() / 2) - 1 < m_weightIndex)
		m_weightIndex = 0;

	if (m_brushArrTex->GetArrSize() - 1 < m_brushIndex)
		m_brushIndex = 0;

	if (KEY_TAP(KEY::RIGHT) && TERRAIN_MODE::SPLAT == m_mode)
	{
		++m_weightIndex;

		if ((m_brushArrTex->GetArrSize() / 2) - 1 < m_weightIndex)
			m_weightIndex = 0;
	}

	if (KEY_HOLD(KEY::LBUTTON))
	{
		if (TERRAIN_MODE::SPLAT == m_mode)
		{
			m_CSWeight->SetWeightMap(m_weightMap, m_weightWidth, m_weightHeight);
			m_CSWeight->SetBrushArrTex(m_brushArrTex);
			m_CSWeight->SetInputBuffer(m_crossBuffer);
			m_CSWeight->SetInputIndexBuffer(m_arrIndexBuffer);
			m_CSWeight->SetBrushScale(m_brushScale);
			m_CSWeight->SetBrushIndex(m_brushIndex);
			m_CSWeight->SetWeightIndex(m_weightIndex);
			m_CSWeight->Excute();
		}
		else if (TERRAIN_MODE::HEIGHT_MAP_UP == m_mode || TERRAIN_MODE::HEIGHT_MAP_DOWN == m_mode || TERRAIN_MODE::HEIGHT_MAP_FLATTENING == m_mode)
		{
			int mode;
			if (TERRAIN_MODE::HEIGHT_MAP_UP == m_mode)
				mode = 0;
			else if (TERRAIN_MODE::HEIGHT_MAP_DOWN == m_mode)
				mode = 1;
			else
				mode = 2;

			m_CSHeight->SetHeightMap(m_heightMap);
			m_CSHeight->SetBrushTexture(m_brushArrTex);
			m_CSHeight->SetInputBuffer(m_crossBuffer);
			m_CSHeight->SetInputIndexBuffer(m_arrIndexBuffer);
			m_CSHeight->SetBrushScale(m_brushScale);
			m_CSHeight->SetBrushIndex(m_brushIndex);
			m_CSHeight->SetHeightMapMode(mode);

			m_CSHeight->Excute();
		}
	}
}

void CTerrain::render()
{
	if (nullptr == m_mesh || nullptr == m_material)
		return;

	Transform()->UpdateData();

	if (m_bWireFrame)
		m_material->GetShader()->SetRSType(RASTERIZER_TYPE::WIREFRAME);
	else
		m_material->GetShader()->SetRSType(RASTERIZER_TYPE::CULL_BACK);

	m_material->SetData(SHADER_PARAM::INT_0, &m_faceX);
	m_material->SetData(SHADER_PARAM::INT_1, &m_faceZ);

	float tileCount = (float)(m_tileArrTex->GetArrSize() / 2.f);
	m_material->SetData(SHADER_PARAM::FLOAT_0, &tileCount);
	m_material->SetData(SHADER_PARAM::FLOAT_1, &m_maxTessLevel);
	m_material->SetData(SHADER_PARAM::TEXTURE_ARR_1, m_tileArrTex.Get());

	Vec2 weightMapResolution = Vec2((float)m_weightWidth, (float)m_weightHeight);
	m_material->SetData(SHADER_PARAM::VEC2_1, &weightMapResolution);
	m_material->SetData(SHADER_PARAM::VEC2_2, &m_tessDistMinMax);
	m_weightMap->UpdateData(14);

	int bVisible = 0;

	if (TERRAIN_MODE::NONE != m_mode)
	{
		m_material->SetData(SHADER_PARAM::INT_2, &m_brushIndex);
		bVisible = 1;
		m_material->SetData(SHADER_PARAM::INT_3, &bVisible);
		m_material->SetData(SHADER_PARAM::VEC2_0, &m_brushScale);
		m_material->SetData(SHADER_PARAM::TEXTURE_ARR_0, m_brushArrTex.Get());
		m_crossBuffer->UpdateData(13, SHADER_STAGE::Pixel);
	}
	else
	{
		bVisible = 0;
		m_material->SetData(SHADER_PARAM::INT_3, &bVisible);
	}

	CRenderManager::GetInst()->GetLightMergeMaterial()->SetData(SHADER_PARAM::INT_0, &bVisible);

	m_material->UpdateData();

	m_mesh->UpdateData(0);

	m_mesh->render(0);

	m_material->Clear();
	m_crossBuffer->Clear();
	m_weightMap->Clear();
}

ULONG64 CTerrain::GetInstID()
{
	if (m_mesh == NULL || m_material == NULL)
		return 0;

	uInstID id{ (UINT)m_mesh->GetID(), (WORD)m_material->GetID(), (WORD)0 };

	return id.ID;
}

void CTerrain::SetFaceCount(UINT _faceX, UINT _faceY)
{
	if (_faceX == m_faceX && _faceY == m_faceZ)
		return;

	m_faceX = _faceX;
	m_faceZ = _faceY;

	CreateTerrainMesh();
}

void CTerrain::CreateTerrainMesh()
{
	vector<tVertex> vectorVertex;
	vector<UINT> vecIndex;

	tVertex vertex;

	for (UINT col = 0; col < m_faceZ + 1; ++col)
	{
		for (UINT row = 0; row < m_faceX + 1; ++row)
		{
			vertex.pos = Vec3((float)row, 0.f, (float)col);
			vertex.UV = Vec2((float)row, m_faceZ - (float)col);

			vertex.normal = Vec3(0.f, 1.f, 0.f);
			vertex.tangent = Vec3(1.f, 0.f, 0.f);
			vertex.binormal = Vec3(0.f, 0.f, -1.f);

			vectorVertex.push_back(vertex);
		}
	}

	for (UINT col = 0; col < m_faceZ; ++col)
	{
		for (UINT row = 0; row < m_faceX; ++row)
		{
			vecIndex.push_back(col * (m_faceX + 1) + row + m_faceX + 1);
			vecIndex.push_back(col * (m_faceX + 1) + row + 1);
			vecIndex.push_back(col * (m_faceX + 1) + row);

			vecIndex.push_back(col * (m_faceX + 1) + row + 1);
			vecIndex.push_back(col * (m_faceX + 1) + row + m_faceX + 1);
			vecIndex.push_back(col * (m_faceX + 1) + row + m_faceX + 2);
		}
	}

	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"TerrainMesh");

	if (nullptr == m_mesh)
	{
		m_mesh = new CMesh;
		m_mesh->Create(vectorVertex.data(), (UINT)(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), (UINT)(sizeof(UINT) * vecIndex.size()));
		CResourceManager::GetInst()->AddRes(L"TerrainMesh", m_mesh.Get());
	}
	else
	{
		m_mesh->Reset(vectorVertex.data(), (UINT)(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), (UINT)(sizeof(UINT) * vecIndex.size()));
	}
}

void CTerrain::SaveToScene(FILE* _file)
{
}

void CTerrain::LoadFromScene(FILE* _file)
{
}