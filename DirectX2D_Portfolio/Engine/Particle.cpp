#include "pch.h"
#include "Particle.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "StructuredBuffer.h"
#include "Transform.h"

Particle::Particle() :
	Component(COMPONENT_TYPE::PARTICLE),
	m_textureIndex(0),
	m_particleBuffer(nullptr),
	m_startColor(Vector4(0.f, 0.f, 0.f, 1.f)),
	m_endColor(Vector4(0.f, 0.f, 0.f, 1.f)),
	m_startScale(Vector3(1.f, 1.f, 0.f)),
	m_endScale(Vector3(1.f, 1.f, 0.f)),
	m_startSpeed(0.f),
	m_endSpeed(0.f),
	m_minLifeTime(1.f),
	m_maxLifeTime(1.f),
	m_spawnRate(1),
	m_maxParticleCount(1),
	m_area(Vector3(1.f, 1.f, 0.f)),
	m_accTime(0.f)
{
	m_mesh = ResourceManager::GetInst()->FindRes<Mesh>(L"PointMesh");
	m_material = ResourceManager::GetInst()->FindRes<Material>(L"ParticleMaterial");
	m_updateShader = (ParticleUpdateShader*)ResourceManager::GetInst()->FindRes<ComputeShader>(L"ParticleUpdateShader").Get();

	m_particleBuffer = new StructuredBuffer;
	m_particleBuffer->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(tParticle), m_maxParticleCount, nullptr);

	assert(m_mesh.Get());
	assert(m_material.Get());
}

Particle::Particle(const Particle& _origin) :
	Component(_origin),
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
	m_mesh = ResourceManager::GetInst()->FindRes<Mesh>(L"PointMesh");
	m_material = ResourceManager::GetInst()->FindRes<Material>(L"ParticleMaterial");
	m_updateShader = (ParticleUpdateShader*)ResourceManager::GetInst()->FindRes<ComputeShader>(L"ParticleUpdateShader").Get();

	m_particleBuffer = new StructuredBuffer;
	m_particleBuffer->Create(STRUCTURED_TYPE::READ_WRITE, sizeof(tParticle), m_maxParticleCount, nullptr);

	assert(m_mesh.Get());
	assert(m_material.Get());
}

Particle::~Particle()
{
	Safe_Delete(m_particleBuffer);
}

void Particle::start()
{
}

void Particle::update()
{
}

void Particle::lateUpdate()
{
	int spawnCount = CalSpawnCount();

	m_updateShader->SetParticleBuffer(m_particleBuffer);
	m_updateShader->SetSpawnCount(spawnCount);
	m_updateShader->SetStartSpeed(m_startSpeed);
	m_updateShader->SetEndSpeed(m_endSpeed);
	m_updateShader->SetMinLife(m_minLifeTime);
	m_updateShader->SetMaxLife(m_maxLifeTime);
	m_updateShader->SetArea(m_area);
	m_updateShader->SetWorldPos(GetTransform()->GetWorldPos());
	m_updateShader->Execute();
}

void Particle::render()
{
	if(!m_vectorParticleTexture.empty())
		m_material->SetData(SHADER_PARAM::TEXTURE_0, m_vectorParticleTexture[m_textureIndex].Get());
	m_material->SetData(SHADER_PARAM::VECTOR4_0, &m_startScale);
	m_material->SetData(SHADER_PARAM::VECTOR4_1, &m_endScale);
	m_material->SetData(SHADER_PARAM::VECTOR4_2, &m_startColor);
	m_material->SetData(SHADER_PARAM::VECTOR4_3, &m_endColor);
	m_material->UpdateData();

	GetTransform()->UpdateData();
	m_particleBuffer->UpdateData(12);
	m_mesh->UpdateData();

	m_mesh->render_particle(m_maxParticleCount);
	m_material->Clear();
	m_particleBuffer->Clear();
}

UINT Particle::CalSpawnCount()
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

void Particle::SaveToScene(FILE* _file)
{
	Component::SaveToScene(_file);
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

void Particle::LoadFromScene(FILE* _file)
{
	Component::LoadFromScene(_file);
	UINT textureCount = 0;
	LoadFromFile(&textureCount, _file);

	m_vectorParticleTexture.clear();
	for (UINT i = 0; i < textureCount; ++i)
	{
		Ptr<Texture> tex = nullptr;
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
