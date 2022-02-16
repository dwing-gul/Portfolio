#include "pch.h"
#include "ParticleUpdateShader.h"
#include "Device.h"
#include "StructuredBuffer.h"
#include "ConstBuffer.h"

ParticleUpdateShader::ParticleUpdateShader() :
	ComputeShader(1024, 1, 1),
	m_particleBuffer(nullptr),
	m_spawnCount(0),
	m_objPos(Vector3(0.f, 0.f, 0.f)),
	m_area(Vector3(0.f, 0.f, 0.f)),
	m_startSpeed(0.f),
	m_endSpeed(0.f),
	m_minLifeTime(0.f),
	m_maxLifeTime(0.f)
{
	m_sharedBuffer = new StructuredBuffer;
	m_sharedBuffer->Create(STRUCTURED_TYPE::DUAL, sizeof(tSharedData), 1, nullptr);
}

ParticleUpdateShader::~ParticleUpdateShader()
{
	Safe_Delete(m_sharedBuffer);
}

void ParticleUpdateShader::UpdateData()
{
	m_particleBuffer->UpdateDataRW(0);

	tSharedData shared = {};
	shared.spawnCount = m_spawnCount;
	m_sharedBuffer->SetData(&shared, 1);
	m_sharedBuffer->UpdateDataRW(1);

	m_info.intArr[0] = m_particleBuffer->GetElementCount();
	m_info.floatArr[0] = m_startSpeed;
	m_info.floatArr[1] = m_endSpeed;
	m_info.floatArr[2] = m_minLifeTime;
	m_info.floatArr[3] = m_maxLifeTime;
	m_info.vector4Arr[0] = m_objPos;
	m_info.vector4Arr[1] = m_area;

	g_materialBuffer->SetData(&m_info);
	g_materialBuffer->UpdateData(SHADER_STAGE::Compute);
}

void ParticleUpdateShader::Clear()
{
	m_particleBuffer->ClearRW();
	m_sharedBuffer->ClearRW();
	g_materialBuffer->Clear(SHADER_STAGE::Compute);
}

void ParticleUpdateShader::Execute()
{
	UINT elementCount = m_particleBuffer->GetElementCount();
	UINT threadX = (elementCount / GetThreadX()) + 1;
	Dispatch(threadX, 1, 1);
}
