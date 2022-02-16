#include "pch.h"
#include "CParticleSystem.h"
#include "CResourceManager.h"
#include "CTimeManager.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"

CParticleSystem::CParticleSystem() :
	CComponent(COMPONENT_TYPE::PARTICLE),
	m_textureIndex(1),
	m_particleBuffer(nullptr),
	m_startColor(Vec4(0.9f, 0.725f, 0.214f, 1.f)),
	m_endColor(Vec4(0.9f, 0.1f, 0.414f, 1.f)),
	m_startScale(Vec3(50.f, 50.f, 0.f)),
	m_endScale(Vec3(20.f, 20.f, 0.f)),
	m_startSpeed(100.f),
	m_endSpeed(10.f),
	m_minLifeTime(1.f),
	m_maxLifeTime(4.f),
	m_spawnRate(100),
	m_maxParticleCount(1000),
	m_area(Vec3(50.f, 50.f, 0.f)),
	m_accTime(0.f)
{
	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"ParticleMaterial");
	m_updateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_particleBuffer = new CStructuredBuffer;
	m_particleBuffer->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(tParticle), m_maxParticleCount, nullptr);

	assert(m_mesh.Get());
	assert(m_material.Get());

	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle01", L"texture\\particle\\AlphaCircle.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle02", L"texture\\particle\\Bubbles99px.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle03", L"texture\\particle\\HardCircle.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle04", L"texture\\particle\\HardRain.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle05", L"texture\\particle\\smokeparticle.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle06", L"texture\\particle\\Snow50px.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle07", L"texture\\particle\\Sparks.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle08", L"texture\\particle\\Fx_StoneAB_02.png"));
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin) :
	CComponent(_origin),
	m_textureIndex(_origin.m_textureIndex),
	m_mesh(nullptr),
	m_material(nullptr),
	m_particleBuffer(nullptr),
	m_updateShader(nullptr),
	m_startColor(_origin.m_startColor),
	m_endColor(_origin.m_endColor),
	m_startScale(_origin.m_startScale),
	m_endScale(_origin.m_endScale),
	m_startSpeed(_origin.m_startSpeed),
	m_endSpeed(_origin.m_endSpeed),
	m_minLifeTime(_origin.m_minLifeTime),
	m_maxLifeTime(_origin.m_maxLifeTime),
	m_spawnRate(_origin.m_spawnRate),
	m_maxParticleCount(_origin.m_maxParticleCount),
	m_area(_origin.m_area),
	m_accTime(_origin.m_accTime)
{
	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"ParticleMaterial");
	m_updateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_particleBuffer = new CStructuredBuffer;
	m_particleBuffer->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(tParticle), m_maxParticleCount, nullptr);

	assert(m_mesh.Get());
	assert(m_material.Get());

	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle01", L"texture\\particle\\AlphaCircle.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle02", L"texture\\particle\\Bubbles99px.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle03", L"texture\\particle\\HardCircle.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle04", L"texture\\particle\\HardRain.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle05", L"texture\\particle\\smokeparticle.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle06", L"texture\\particle\\Snow50px.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle07", L"texture\\particle\\Sparks.png"));
	m_vectorParticleTexture.push_back(CResourceManager::GetInst()->Load<CTexture>(L"Particle08", L"texture\\particle\\CartoonSmoke.png"));
}

CParticleSystem::~CParticleSystem()
{
	Safe_Delete_Ptr(m_particleBuffer);	
}

void CParticleSystem::start()
{

}

void CParticleSystem::update()
{
}

void CParticleSystem::lateUpdate()
{
	int spawnCount = CalSpawnCount();

	m_updateShader->SetParticleBuffer(m_particleBuffer);
	m_updateShader->SetSpawnCount(spawnCount);
	m_updateShader->SetStartSpeed(m_startSpeed);
	m_updateShader->SetEndSpeed(m_endSpeed);
	m_updateShader->SetMinLife(m_minLifeTime);
	m_updateShader->SetMaxLife(m_maxLifeTime);
	m_updateShader->SetArea(m_area);
	m_updateShader->SetWorldPos(Transform()->GetWorldPos());
	m_updateShader->Excute();
}

void CParticleSystem::render()
{
	m_material->SetData(SHADER_PARAM::TEXTURE_0, m_vectorParticleTexture[m_textureIndex].Get());
	m_material->SetData(SHADER_PARAM::VEC4_0, &m_startScale);
	m_material->SetData(SHADER_PARAM::VEC4_1, &m_endScale);
	m_material->SetData(SHADER_PARAM::VEC4_2, &m_startColor);
	m_material->SetData(SHADER_PARAM::VEC4_3, &m_endColor);
	m_material->UpdateData();

	Transform()->UpdateData();
	m_particleBuffer->UpdateData(12, SHADER_STAGE::ALL);

	m_mesh->UpdateData(0);

	m_mesh->render_particle(m_maxParticleCount);
	m_material->Clear();
	m_particleBuffer->Clear(SHADER_STAGE::ALL);
}

UINT CParticleSystem::CalSpawnCount()
{
	UINT spawnCount = 0;
	float createTime = 1.f / (float)m_spawnRate;
	m_accTime += DELTATIME;

	if (createTime < m_accTime)
	{
		float createCount = m_accTime / createTime;
		spawnCount = (UINT)floorf(createCount);
		m_accTime -= (createTime * (float)spawnCount);
	}

	return spawnCount;
}

void CParticleSystem::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	UINT textureCount = (UINT)m_vectorParticleTexture.size();
	SaveToFile(&textureCount, _file);

	for (size_t i = 0; i < m_vectorParticleTexture.size(); ++i)
	{
		SaveResourceToFile(m_vectorParticleTexture[i], _file);
	}

	SaveToFile(&m_textureIndex, _file);
	SaveToFile(&m_startColor, _file);
	SaveToFile(&m_endColor, _file);
	SaveToFile(&m_startScale, _file);
	SaveToFile(&m_endScale, _file);
	SaveToFile(&m_startSpeed, _file);
	SaveToFile(&m_endSpeed, _file);
	SaveToFile(&m_minLifeTime, _file);
	SaveToFile(&m_maxLifeTime, _file);
	SaveToFile(&m_spawnRate, _file);
	SaveToFile(&m_maxParticleCount, _file);
	SaveToFile(&m_area, _file);
}

void CParticleSystem::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	UINT textureCount = 0;
	LoadFromFile(&textureCount, _file);

	m_vectorParticleTexture.clear();
	for (UINT i = 0; i < textureCount; ++i)
	{
		Ptr<CTexture> tex = nullptr;
		LoadResourceFromFile(tex, _file);
		m_vectorParticleTexture.push_back(tex);
	}

	LoadFromFile(&m_textureIndex, _file);
	LoadFromFile(&m_startColor, _file);
	LoadFromFile(&m_endColor, _file);
	LoadFromFile(&m_startScale, _file);
	LoadFromFile(&m_endScale, _file);
	LoadFromFile(&m_startSpeed, _file);
	LoadFromFile(&m_endSpeed, _file);
	LoadFromFile(&m_minLifeTime, _file);
	LoadFromFile(&m_maxLifeTime, _file);
	LoadFromFile(&m_spawnRate, _file);
	LoadFromFile(&m_maxParticleCount, _file);
	LoadFromFile(&m_area, _file);

	if (m_maxParticleCount != m_particleBuffer->GetElementCount())
	{
		m_particleBuffer->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(tParticle), m_maxParticleCount);
	}
}
