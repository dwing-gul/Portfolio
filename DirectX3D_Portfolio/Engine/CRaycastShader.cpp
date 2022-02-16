#include "pch.h"
#include "CRaycastShader.h"
#include "CStructuredBuffer.h"
#include "CConstBuffer.h"

CRaycastShader::CRaycastShader() :
	CComputeShader(32, 32, 1),
	m_output(nullptr),
	m_index(nullptr),
	m_xFace(1),
	m_zFace(1)
{
}

CRaycastShader::~CRaycastShader()
{
}

void CRaycastShader::UpdateData()
{
	m_info.intArr[0] = m_xFace;
	m_info.intArr[1] = m_zFace;

	m_info.vec4Arr[0] = m_ray.start;
	m_info.vec4Arr[1] = m_ray.dir;

	g_materialBuffer->SetData(&m_info);
	g_materialBuffer->UpdateData(SHADER_STAGE::Compute);

	m_heightMap->UpdateData(SHADER_STAGE::Compute, 0);

	m_output->UpdateDataRW(0);
	m_index->UpdateDataRW(1);
}

void CRaycastShader::Clear()
{
	g_materialBuffer->Clear();
	m_heightMap->Clear(0);
	m_output->ClearRW();
	m_index->ClearRW();
}

void CRaycastShader::Excute()
{
	UINT groupX = ((m_xFace * 2) / GetThreadX()) + 1;
	UINT groupY = (m_zFace / GetThreadY()) + 1;

	Dispatch(groupX, groupY, 1);
}
