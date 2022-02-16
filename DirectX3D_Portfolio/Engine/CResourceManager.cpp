#include "pch.h"
#include "CResourceManager.h"
#include "CParticleUpdateShader.h"
#include "CCopyTextureShader.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"
#include "CAnimation3DShader.h"
#include "CCopyBoneMatrixShader.h"

CResourceManager::CResourceManager() :
	m_bEvent(false)
{
}

CResourceManager::~CResourceManager()
{
	Safe_Delete_Vec(m_vecCloneMaterial);

	for (UINT i = 0; i < (UINT)RESOURCE_TYPE::END; ++i)
	{
		Safe_Delete_Map(m_mapRes[i]);
	}
}

void CResourceManager::init()
{
	CreateDefaultMesh();
	CreateDefaultTexture();
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateComputeShader();
	InitSound();
}

void CResourceManager::CreateDefaultMesh()
{
	CMesh* mesh = nullptr;

	vector<tVertex> vectorVertex;
	vector<UINT> vecIndex;

	tVertex vertex;

	vertex.pos = Vec3(0.f, 0.f, 0.f);
	vertex.UV = Vec2(0.f, 0.f);
	vectorVertex.push_back(vertex);
	vecIndex.push_back(0);

	mesh = new CMesh;
	mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), UINT(sizeof(UINT) * vecIndex.size()));

	AddRes(L"PointMesh", mesh);

	vectorVertex.clear();
	vecIndex.clear();

	vertex.pos = Vec3(-0.5f, 0.5f, 0.5f);
	vertex.UV = Vec2(0.f, 0.f);

	vertex.tangent = Vec3(1.f, 0.f, 0.f);
	vertex.normal = Vec3(0.f, 0.f, -1.f);
	vertex.binormal = Vec3(0.f, 1.f, 0.f);

	vectorVertex.push_back(vertex);

	vertex.pos = Vec3(0.5f, 0.5f, 0.5f);
	vertex.UV = Vec2(1.f, 0.f);
	vectorVertex.push_back(vertex);

	vertex.pos = Vec3(0.5f, -0.5f, 0.5f);
	vertex.UV = Vec2(1.f, 1.f);
	vectorVertex.push_back(vertex);

	vertex.pos = Vec3(-0.5f, -0.5f, 0.5f);
	vertex.UV = Vec2(0.f, 1.f);
	vectorVertex.push_back(vertex);

	vecIndex.push_back(0); vecIndex.push_back(1); vecIndex.push_back(2);
	vecIndex.push_back(0); vecIndex.push_back(2); vecIndex.push_back(3);

	mesh = new CMesh;
	mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), UINT(sizeof(UINT) * vecIndex.size()));

	AddRes(L"RectMesh", mesh);

	vecIndex.clear();
	vecIndex.push_back(0); vecIndex.push_back(1); vecIndex.push_back(2); vecIndex.push_back(3); vecIndex.push_back(0);

	mesh = new CMesh;
	mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), UINT(sizeof(UINT) * vecIndex.size()));

	AddRes(L"RectLineMesh", mesh);

	vectorVertex.clear();
	vecIndex.clear();

	float radius = 0.5f;
	int divide = 40;
	float theta = XM_2PI / (float)divide;

	vertex.pos = Vec3(0.f, 0.f, 0.f);
	vertex.UV = Vec2(0.5f, 0.5f);

	vertex.tangent = Vec3(1.f, 0.f, 0.f);
	vertex.normal = Vec3(0.f, 0.f, -1.f);
	vertex.binormal = Vec3(0.f, 1.f, 0.f);

	vectorVertex.push_back(vertex);

	for (int i = 0; i < divide + 1; ++i)
	{
		vertex.pos = Vec3(radius * cosf(theta * (float)i), radius * sinf(theta * (float)i), 0.f);
		vertex.UV = Vec2((vertex.pos.x + radius) / (2 * radius), (-vertex.pos.y + radius) / (2 * radius));
		vectorVertex.push_back(vertex);
	}

	for (int i = 0; i < divide; ++i)
	{
		vecIndex.push_back(0);
		vecIndex.push_back(i + 2);
		vecIndex.push_back(i + 1);
	}

	mesh = new CMesh;
	mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), UINT(sizeof(UINT) * vecIndex.size()));
	AddRes(L"CircleMesh", mesh);

	vecIndex.clear();
	for (int i = 0; i < divide + 1; ++i)
	{
		vecIndex.push_back(i + 1);
	}

	mesh = new CMesh;
	mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), UINT(sizeof(UINT) * vecIndex.size()));
	AddRes(L"CircleLineMesh", mesh);

	vectorVertex.clear();
	vecIndex.clear();

	tVertex arrCube[24] = {};
	// À­¸é
	arrCube[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].UV = Vec2(0.f, 0.f);
	arrCube[0].normal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].UV = Vec2(0.f, 0.f);
	arrCube[1].normal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].pos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].UV = Vec2(0.f, 0.f);
	arrCube[2].normal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].pos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].UV = Vec2(0.f, 0.f);
	arrCube[3].normal = Vec3(0.f, 1.f, 0.f);


	// ¾Æ·§ ¸é	
	arrCube[4].pos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].UV = Vec2(0.f, 0.f);
	arrCube[4].normal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].pos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].UV = Vec2(0.f, 0.f);
	arrCube[5].normal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].pos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].UV = Vec2(0.f, 0.f);
	arrCube[6].normal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].pos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].UV = Vec2(0.f, 0.f);
	arrCube[7].normal = Vec3(0.f, -1.f, 0.f);

	// ¿ÞÂÊ ¸é
	arrCube[8].pos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].UV = Vec2(0.f, 0.f);
	arrCube[8].normal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].pos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].UV = Vec2(0.f, 0.f);
	arrCube[9].normal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].pos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].UV = Vec2(0.f, 0.f);
	arrCube[10].normal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].pos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].UV = Vec2(0.f, 0.f);
	arrCube[11].normal = Vec3(-1.f, 0.f, 0.f);

	// ¿À¸¥ÂÊ ¸é
	arrCube[12].pos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].UV = Vec2(0.f, 0.f);
	arrCube[12].normal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].pos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].UV = Vec2(0.f, 0.f);
	arrCube[13].normal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].pos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].UV = Vec2(0.f, 0.f);
	arrCube[14].normal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].pos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].UV = Vec2(0.f, 0.f);
	arrCube[15].normal = Vec3(1.f, 0.f, 0.f);

	// µÞ ¸é
	arrCube[16].pos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].UV = Vec2(0.f, 0.f);
	arrCube[16].normal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].pos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].UV = Vec2(0.f, 0.f);
	arrCube[17].normal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].pos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].UV = Vec2(0.f, 0.f);
	arrCube[18].normal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].pos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].UV = Vec2(0.f, 0.f);
	arrCube[19].normal = Vec3(0.f, 0.f, 1.f);

	// ¾Õ ¸é
	arrCube[20].pos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[20].UV = Vec2(0.f, 0.f);
	arrCube[20].normal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].pos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].UV = Vec2(0.f, 0.f);
	arrCube[21].normal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].pos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].UV = Vec2(0.f, 0.f);
	arrCube[22].normal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].pos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].UV = Vec2(0.f, 0.f);
	arrCube[23].normal = Vec3(0.f, 0.f, -1.f);

	// ÀÎµ¦½º
	for (int i = 0; i < 12; i += 2)
	{
		vecIndex.push_back(i * 2);
		vecIndex.push_back(i * 2 + 1);
		vecIndex.push_back(i * 2 + 2);

		vecIndex.push_back(i * 2);
		vecIndex.push_back(i * 2 + 2);
		vecIndex.push_back(i * 2 + 3);
	}

	mesh = new CMesh;
	mesh->Create(arrCube, sizeof(tVertex) * 24, vecIndex.data(), sizeof(UINT) * (UINT)vecIndex.size());
	AddRes(L"CubeMesh", mesh);

	vecIndex.clear();

	vecIndex.push_back(0); vecIndex.push_back(1); vecIndex.push_back(2); vecIndex.push_back(3); vecIndex.push_back(0);
	vecIndex.push_back(7); vecIndex.push_back(6); vecIndex.push_back(5); vecIndex.push_back(4); vecIndex.push_back(7);
	vecIndex.push_back(0); vecIndex.push_back(3); vecIndex.push_back(4); vecIndex.push_back(5); vecIndex.push_back(2);
	vecIndex.push_back(1); vecIndex.push_back(6);

	mesh = new CMesh;
	mesh->Create(arrCube, sizeof(tVertex) * 24, vecIndex.data(), sizeof(UINT) * (UINT)vecIndex.size());
	AddRes(L"CubeLineMesh", mesh);

	vectorVertex.clear();
	vecIndex.clear();

	// ===============
	// Sphere Mesh(±¸)
	// ===============
	radius = 1.f;

	// Top
	vertex.pos = Vec3(0.f, radius, 0.f);
	vertex.UV = Vec2(0.5f, 0.f);
	vertex.normal = vertex.pos;
	vertex.normal.Normalize();
	vertex.tangent = Vec3(1.f, 0.f, 0.f);
	vertex.binormal = Vec3(0.f, 0.f, 1.f);
	vectorVertex.push_back(vertex);

	// Body
	UINT stackCount = 40; // °¡·Î ºÐÇÒ °³¼ö
	UINT sliceCount = 40; // ¼¼·Î ºÐÇÒ °³¼ö

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float uvXStep = 1.f / (float)sliceCount;
	float uvYStep = 1.f / (float)stackCount;

	for (UINT i = 1; i < stackCount; ++i)
	{
		float phi = i * stackAngle;

		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * sliceAngle;

			vertex.pos = Vec3(radius * sinf(i * stackAngle) * cosf(j * sliceAngle)
				, radius * cosf(i * stackAngle)
				, radius * sinf(i * stackAngle) * sinf(j * sliceAngle));
			vertex.UV = Vec2(uvXStep * j, uvYStep * i);
			vertex.normal = vertex.pos;
			vertex.normal.Normalize();

			vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
			vertex.tangent.y = 0.f;
			vertex.tangent.z = radius * sinf(phi) * cosf(theta);
			vertex.tangent.Normalize();

			vertex.tangent.Cross(vertex.normal, vertex.binormal);
			vertex.binormal.Normalize();

			vectorVertex.push_back(vertex);
		}
	}

	// Bottom
	vertex.pos = Vec3(0.f, -radius, 0.f);
	vertex.UV = Vec2(0.5f, 1.f);
	vertex.normal = vertex.pos;
	vertex.normal.Normalize();

	vertex.tangent = Vec3(1.f, 0.f, 0.f);
	vertex.binormal = Vec3(0.f, 0.f, -1.f);
	vectorVertex.push_back(vertex);

	// ÀÎµ¦½º
	// ºÏ±ØÁ¡
	for (UINT i = 0; i < sliceCount; ++i)
	{
		vecIndex.push_back(0);
		vecIndex.push_back(i + 2);
		vecIndex.push_back(i + 1);
	}

	// ¸öÅë
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIndex.push_back((sliceCount + 1) * (i)+(j)+1);
			vecIndex.push_back((sliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIndex.push_back((sliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIndex.push_back((sliceCount + 1) * (i)+(j)+1);
			vecIndex.push_back((sliceCount + 1) * (i)+(j + 1) + 1);
			vecIndex.push_back((sliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// ³²±ØÁ¡
	UINT bottomIdx = (UINT)vectorVertex.size() - 1;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		vecIndex.push_back(bottomIdx);
		vecIndex.push_back(bottomIdx - (i + 2));
		vecIndex.push_back(bottomIdx - (i + 1));
	}

	mesh = new CMesh;
	mesh->Create(vectorVertex.data(), sizeof(tVertex) * (UINT)vectorVertex.size(), vecIndex.data(), sizeof(UINT) * (UINT)vecIndex.size());
	mesh->SetName(L"SphereMesh");
	AddRes(mesh->GetName(), mesh);

	vectorVertex.clear();
	vecIndex.clear();

	radius = 0.5f;
	divide = 40;
	theta = XM_2PI / (float)divide;

	// ¿ø»ÔÀÇ ²ÀÁþÁ¡
	vertex.pos = Vec3(0.f, 1.f, 0.f);
	vertex.UV = Vec2(0.5f, 0.5f);

	vectorVertex.push_back(vertex);

	// ¿ø»ÔÀÇ ¹Ø ¿ø
	vertex.pos = Vec3(0.f, 0.f, 0.f);
	vertex.UV = Vec2(0.5f, 0.5f);

	vertex.tangent = Vec3(1.f, 0.f, 0.f);
	vertex.normal = Vec3(0.f, 0.f, -1.f);
	vertex.binormal = Vec3(0.f, 1.f, 0.f);

	vectorVertex.push_back(vertex);

	for (int i = 0; i < divide + 1; ++i)
	{
		vertex.pos = Vec3(radius * cosf(theta * (float)i), 0.f, radius * sinf(theta * (float)i));
		vertex.UV = Vec2((vertex.pos.x + radius) / (2 * radius), (-vertex.pos.y + radius) / (2 * radius));
		vectorVertex.push_back(vertex);
	}

	for (int i = 1; i < divide + 1; ++i)
	{
		vecIndex.push_back(1);
		vecIndex.push_back(i + 1);
		vecIndex.push_back(i + 2);
	}

	vecIndex.push_back(0);

	for (int i = 0; i < divide; ++i)
	{
		vecIndex.push_back(0);
		vecIndex.push_back(i + 3);
		vecIndex.push_back(i + 2);
	}

	mesh = new CMesh;
	mesh->Create(vectorVertex.data(), UINT(sizeof(tVertex) * vectorVertex.size()), vecIndex.data(), UINT(sizeof(UINT) * vecIndex.size()));
	AddRes(L"ConeMesh", mesh);

	vectorVertex.clear();
	vecIndex.clear();
}

void CResourceManager::CreateDefaultTexture()
{
	Ptr<CTexture> tex = CResourceManager::GetInst()->Load<CTexture>(L"noise_01", L"texture\\noise\\noise_01.png");
	CResourceManager::GetInst()->Load<CTexture>(L"noise_02", L"texture\\noise\\noise_02.png");

	tex->UpdateData(SHADER_STAGE::ALL, 54);
	g_global.noiseResolution = Vec2((float)tex->Width(), (float)tex->Height());
}

void CResourceManager::CreateDefaultShader()
{
	CGraphicsShader* shader = nullptr;

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	shader->AddShaderParam(SHADER_PARAM::INT_0, L"Test Param Int");
	shader->AddShaderParam(SHADER_PARAM::FLOAT_0, L"Test Param Float");
	shader->AddShaderParam(SHADER_PARAM::VEC2_0, L"Test Param Vec2");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"Test Param Vec4");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Output Texture1");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Output Texture2");

	AddRes(L"Std2DShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D_Light");
	shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Light");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	AddRes(L"Std2DLightShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND);

	AddRes(L"Std2D_AlphaBlendShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\bar.fx", "VS_Bar");
	shader->CreatePixelShader(L"shader\\bar.fx", "PS_Bar");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	shader->AddShaderParam(SHADER_PARAM::INT_0, L"Dir");
	shader->AddShaderParam(SHADER_PARAM::INT_1, L"Use");
	shader->AddShaderParam(SHADER_PARAM::FLOAT_0, L"Ratio");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Output Texture1");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Output Texture2");

	AddRes(L"BarShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Collider2D");
	shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Collider2D");

	shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST);

	AddRes(L"Collider2DShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std2d.fx", "VS_TileMap");
	shader->CreatePixelShader(L"shader\\std2d.fx", "PS_TileMap");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);

	AddRes(L"TileMapShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	shader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Diffuse");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Normal Map");

	AddRes(L"Std3DShader", shader);

	shader = new CGraphicsShader(RENDER_POV::DEFERRED);
	shader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	shader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Diffuse");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Normal Map");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_CUBE_0, L"Environmental Map");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"DiffuseColor");

	AddRes(L"Std3DDeferredShader", shader);

	shader = new CGraphicsShader(RENDER_POV::DEFERRED);
	shader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	shader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Diffuse");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Normal Map");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_CUBE_0, L"Environmental Map");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"DiffuseColor");

	AddRes(L"NaviMeshShader", shader);

	shader = new CGraphicsShader(RENDER_POV::DEFERRED);
	shader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	shader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	shader->SetRSType(RASTERIZER_TYPE::CULL_FRONT);
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Diffuse");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Normal Map");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_CUBE_0, L"Environmental Map");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"DiffuseColor");

	AddRes(L"Std3DDeferredShader_CULL_FRONT", shader);

	//======
	//ÀÌÆåÆ®
	//======
	
	//2d ÀÌÆåÆ® ½¦ÀÌ´õ
	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\Effect.fx", "VS_Std2D");
	shader->CreatePixelShader(L"shader\\Effect.fx", "PS_Effect2D");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Texture");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"Color");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND);

	AddRes(L"Effect2DShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\Effect.fx", "VS_Std2D");
	shader->CreatePixelShader(L"shader\\Effect.fx", "PS_Effect2D_Coverage");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Texture");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"Color");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	AddRes(L"Effect2DCoverageShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\Effect.fx", "VS_Std2D");
	shader->CreatePixelShader(L"shader\\Effect.fx", "PS_BuffEffect2D_Coverage");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Texture");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"Color");
	shader->AddShaderParam(SHADER_PARAM::FLOAT_0, L"AlphaRatio");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	AddRes(L"BuffEffect2DCoverageShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\Effect.fx", "VS_Std2D");
	shader->CreatePixelShader(L"shader\\Effect.fx", "PS_Effect2D_Decal");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Texture");

	shader->AddShaderParam(SHADER_PARAM::INT_0, L"Mod");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"EffectColor");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	AddRes(L"Effect2DDecalShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\Effect.fx", "VS_Std2D");
	shader->CreatePixelShader(L"shader\\Effect.fx", "PS_Effect2D_Line");

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Texture");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"EffectColor");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	AddRes(L"Effect2DLineShader", shader);

	//3d ÀÌÆåÆ®

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\Effect.fx", "VS_Effect3D");
	shader->CreatePixelShader(L"shader\\Effect.fx", "PS_Effect3D_Default");
	//shader->SetRSType(RASTERIZER_TYPE::CULL_FRONT);
	//shader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND);

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Diffuse");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Normal Map");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_CUBE_0, L"Environmental Map");
	shader->AddShaderParam(SHADER_PARAM::VEC4_0, L"DiffuseColor");

	AddRes(L"Effect3D_Default", shader);
	

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std3d.fx", "VS_Collider3D");
	shader->CreatePixelShader(L"shader\\std3d.fx", "PS_Collider3D");

	shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST);

	AddRes(L"Collider3DShader", shader);

	shader = new CGraphicsShader(RENDER_POV::DEFERRED);
	shader->CreateVertexShader(L"shader\\terrain.fx", "VS_Terrain");
	shader->CreateHullShader(L"shader\\terrain.fx", "HS_Terrain");
	shader->CreateDomainShader(L"shader\\terrain.fx", "DS_Terrain");
	shader->CreatePixelShader(L"shader\\terrain.fx", "PS_Terrain");

	shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Diffuse");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_1, L"Normal Map");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_2, L"Height Map");

	AddRes(L"TerrainShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\std3d.fx", "VS_SkyBox");
	shader->CreatePixelShader(L"shader\\std3d.fx", "PS_SkyBox");

	shader->SetRSType(RASTERIZER_TYPE::CULL_FRONT);
	shader->SetDSType(DEPTH_STENCIL_TYPE::LESS_EQUAL);

	shader->AddShaderParam(SHADER_PARAM::TEXTURE_CUBE_0, L"SkyBox Image");

	AddRes(L"SkyBoxShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\frustum.fx", "VS_Frustum");
	shader->CreateGeometryShader(L"shader\\frustum.fx", "GS_Frustum");
	shader->CreatePixelShader(L"shader\\frustum.fx", "PS_Frustum");

	shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST);

	AddRes(L"FrustumShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
	shader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
	shader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");

	shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND);
	shader->SetDSType(DEPTH_STENCIL_TYPE::NO_WRITE);

	AddRes(L"ParticleShader", shader);

	shader = new CGraphicsShader(RENDER_POV::POSTEFFECT);
	shader->CreateVertexShader(L"shader\\posteffect.fx", "VS_Distortion");
	shader->CreatePixelShader(L"shader\\posteffect.fx", "PS_Distortion");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetDSType(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE);

	AddRes(L"DistortionShader", shader);

	shader = new CGraphicsShader(RENDER_POV::SHADOW_DEPTH);
	shader->CreateVertexShader(L"shader\\shadow_depth.fx", "VS_ShadowDepth");
	shader->CreatePixelShader(L"shader\\shadow_depth.fx", "PS_ShadowDepth");

	// rasterizer
	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);

	AddRes(L"ShadowDepthShader", shader);

	shader = new CGraphicsShader(RENDER_POV::FORWARD);
	shader->CreateVertexShader(L"shader\\number.fx", "VS_Number");
	shader->CreatePixelShader(L"shader\\number.fx", "PS_Number");

	shader->SetRSType(RASTERIZER_TYPE::CULL_NONE);
	shader->SetBlendType(BLEND_TYPE::ALPHABLEND_COVERAGE);

	shader->AddShaderParam(SHADER_PARAM::INT_0, L"Num Length");
	shader->AddShaderParam(SHADER_PARAM::INT_1, L"Total Num");
	shader->AddShaderParam(SHADER_PARAM::TEXTURE_0, L"Output Texture1");

	AddRes(L"NumberShader", shader);
}

void CResourceManager::CreateDefaultMaterial()
{
	CMaterial* material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"Std2DMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes<CMaterial>(L"Std2DLightMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Std2D_AlphaBlendShader"));
	AddRes<CMaterial>(L"Std2D_AlphaBlendMaterial", material);
	
	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"BarShader"));
	AddRes<CMaterial>(L"BarMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Collider2DShader"));
	AddRes<CMaterial>(L"Collider2DMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"TileMapMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Std3DShader"));
	AddRes<CMaterial>(L"Std3DMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Std3DDeferredShader"));
	AddRes<CMaterial>(L"Std3DDeferredMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"NaviMeshShader"));
	AddRes<CMaterial>(L"NaviMeshMaterial", material);

	//ÀÌÆåÆ® ¸ÞÅ×¸®¾ó

	//2d ÀÌÆåÆ®
	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Effect2DShader"));
	AddRes<CMaterial>(L"Effect2DMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Effect2DCoverageShader"));
	AddRes<CMaterial>(L"Effect2DCoverageMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"BuffEffect2DCoverageShader"));
	AddRes<CMaterial>(L"BuffEffect2DCoverageMaterial", material);
	
	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Effect2DDecalShader"));
	AddRes<CMaterial>(L"Effect2DDecalMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Effect2DLineShader"));
	AddRes<CMaterial>(L"Effect2DLineMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Effect3D_Default"));
	AddRes<CMaterial>(L"Effect3DDefaultMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"Collider3DShader"));
	AddRes<CMaterial>(L"Collider3DMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"TerrainShader"));
	AddRes<CMaterial>(L"TerrainMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"ShadowDepthShader"));
	AddRes<CMaterial>(L"ShadowDepthMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"SkyBoxShader"));
	AddRes<CMaterial>(L"SkyBoxMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"FrustumShader"));
	AddRes<CMaterial>(L"FrustumMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"ParticleShader"));
	AddRes<CMaterial>(L"ParticleMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"DistortionShader"));

	Ptr<CTexture> postEffectTarget = FindRes<CTexture>(L"PostEffectTarget");
	material->SetData(SHADER_PARAM::TEXTURE_0, postEffectTarget.Get());

	AddRes<CMaterial>(L"DistortionMaterial", material);

	material = new CMaterial(true);
	material->SetShader(FindRes<CGraphicsShader>(L"NumberShader"));
	material->SetData(SHADER_PARAM::TEXTURE_0, Load<CTexture>(L"NumberFont", L"texture\\UI\\font\\NumberFont.png").Get());
	AddRes<CMaterial>(L"NumberMaterial", material);
}

void CResourceManager::CreateComputeShader()
{
	CComputeShader* shader = new CParticleUpdateShader;
	shader->CreateComputeShader(L"shader\\particle_update.fx", "CS_ParticleUpdate");
	AddRes(L"ParticleUpdateShader", shader);

	shader = new CCopyTextureShader;
	shader->CreateComputeShader(L"shader\\copytexture.fx", "CS_CopyTexture");
	AddRes(L"CopyTextureShader", shader);

	shader = new CHeightMapShader;
	shader->CreateComputeShader(L"shader\\heightmap.fx", "CS_HeightMap");
	AddRes(L"HeightMapShader", shader);

	shader = new CWeightMapShader;
	shader->CreateComputeShader(L"shader\\weightmap.fx", "CS_WeightMap");
	AddRes(L"WeightMapShader", shader);

	shader = new CRaycastShader;
	shader->CreateComputeShader(L"shader\\raycast.fx", "CS_Raycast");
	AddRes(L"RaycastShader", shader);

	shader = new CAnimation3DShader;
	shader->CreateComputeShader(L"shader\\animation3D.fx", "CS_Animation3D");
	AddRes(L"Animation3DUpdateShader", shader);

	shader = new CCopyBoneMatrixShader;
	shader->CreateComputeShader(L"shader\\bonecopy.fx", "CS_CopyBoneMatrix");
	AddRes(L"CopyBoneMatrixShader", shader);
}

void CResourceManager::InitSound()
{
	FMOD::System_Create(&CSound::g_FMOD);

	if (nullptr == CSound::g_FMOD)
	{
		assert(nullptr);
	}

	// 32°³ Ã¤³Î »ý¼º
	CSound::g_FMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResourceManager::AddRes(const wstring& _key, CRes* _res, RESOURCE_TYPE _type)
{
	_res->SetKey(_key);
	m_mapRes[(UINT)_type].insert(make_pair(_key, _res));
	m_bEvent = true;
}

void CResourceManager::SubRes(const wstring& _key, RESOURCE_TYPE _type)
{
	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)_type].find(_key);
	if (iter == m_mapRes[(UINT)_type].end())
	{
		assert(nullptr);
	}

	m_mapRes[(UINT)_type].erase(iter);
}

Ptr<CTexture> CResourceManager::CreateTexture(const wstring& _key, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag)
{
	assert(nullptr == FindRes<CTexture>(_key));

	CTexture* texture = new CTexture;
	texture->Create(_width, _height, _format, _bindFlag);
	AddRes<CTexture>(_key, texture);
	m_bEvent = true;

	return texture;
}

Ptr<CTexture> CResourceManager::CreateTexture_Tool(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag)
{
	CTexture* texture = new CTexture;
	texture->Create(_width, _height, _format, _bindFlag);
	return texture;
}

Ptr<CTexture> CResourceManager::CreateTexture(const wstring& _key, ComPtr<ID3D11Texture2D> _texture)
{
	assert(nullptr == FindRes<CTexture>(_key));

	CTexture* texture = new CTexture;
	texture->Create(_texture);
	AddRes<CTexture>(_key, texture);
	m_bEvent = true;

	return texture;
}

Ptr<CTexture> CResourceManager::LoadTexture(const wstring& _key, const wstring& _relativePath, int _mipLevel)
{
	CTexture* res = FindRes<CTexture>(_key).Get();

	if (nullptr != res)
	{
		return res;
	}

	res = new CTexture;

	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _relativePath;

	if (FAILED(res->Load(filePath)))
	{
		return nullptr;
	}

	res->SetKey(_key);
	res->SetRelativePath(_relativePath);
	AddRes<CTexture>(_key, res);

	m_bEvent = true;

	return res;
}

Ptr<CMeshData> CResourceManager::LoadFBX(const wstring& _path, bool _bHasAnim)
{
	wstring fileName = path(_path).stem();

	wstring name = L"meshdata\\";
	name += fileName + L".mdat";

	Ptr<CMeshData> meshData = FindRes<CMeshData>(name);

	if (nullptr != meshData)
		return meshData;

	meshData = CMeshData::LoadFromFBX(_path, _bHasAnim);
	meshData->SetKey(name);
	meshData->SetRelativePath(name);
	m_mapRes[(UINT)RESOURCE_TYPE::MESHDATA].insert(make_pair(name, meshData.Get()));

	return meshData;
}

vector<CMeshData*>& CResourceManager::LoadFBX_vector(const wstring& _path, bool _bHasAnim)
{
	// map¿¡ ÀÖ´Â Á¤º¸¸¦ º¤ÅÍ¿¡ ÀúÀåÇÑ´Ù.
	map<wstring, CMeshData*> vecMeshData;

	m_vecMeshData = CMeshData::LoadFromFBX_vector(_path, _bHasAnim);

	return m_vecMeshData;
}

void CResourceManager::GetResName(RESOURCE_TYPE _type, vector<wstring>& _output)
{
	for (const auto& pair : m_mapRes[(UINT)_type])
	{
		_output.push_back(pair.first);
	}
}
