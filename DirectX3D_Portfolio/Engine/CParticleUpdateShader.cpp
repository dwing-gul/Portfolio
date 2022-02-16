#include "pch.h"
#include "CParticleUpdateShader.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CParticleUpdateShader::CParticleUpdateShader() :
	CComputeShader(1024, 1, 1),
	m_particleBuffer(nullptr),
	m_sharedBuffer(nullptr),
	m_spawnCount(0),
	m_objPos(Vec4(0.f, 0.f, 0.f, 0.f)),
	m_area(Vec3(0.f, 0.f, 0.f)),
	m_startSpeed(0.f),
	m_endSpeed(0.f),
	m_minLifeTime(0.f),
	m_maxLifeTime(0.f)
{
	m_sharedBuffer = new CStructuredBuffer;
	m_sharedBuffer->Create(STRUCTURED_TYPE::DUAL, sizeof(tSharedData), 1, nullptr);
}

CParticleUpdateShader::~CParticleUpdateShader()
{
	Safe_Delete_Ptr(m_sharedBuffer);
}

void CParticleUpdateShader::UpdateData()
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

	m_info.vec4Arr[0] = m_objPos;
	m_info.vec4Arr[1] = m_area;

	g_materialBuffer->SetData(&m_info);
	g_materialBuffer->UpdateData(SHADER_STAGE::Compute);
}

void CParticleUpdateShader::Clear()
{
	m_particleBuffer->ClearRW();
	m_sharedBuffer->ClearRW();
	g_materialBuffer->Clear(SHADER_STAGE::Compute);
}

void CParticleUpdateShader::Excute()
{
	UINT elementCount = m_particleBuffer->GetElementCount();
	UINT threadX = (elementCount / GetThreadX()) + 1;
	Dispatch(threadX, 1, 1);
}
