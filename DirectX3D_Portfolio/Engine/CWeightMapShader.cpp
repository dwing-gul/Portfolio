#include "pch.h"
#include "CWeightMapShader.h"
#include "CStructuredBuffer.h"
#include "CConstBuffer.h"

CWeightMapShader::CWeightMapShader() :
	CComputeShader(32, 32, 1),
	m_weightMap(nullptr),
	m_raycastInfo(nullptr),
	m_inputIndex(nullptr),
	m_brushIndex(0),
	m_weightIndex(0),
	m_width(0),
	m_height(0)
{
}

CWeightMapShader::~CWeightMapShader()
{
}

void CWeightMapShader::UpdateData()
{
	m_info.intArr[0] = m_width;
	m_info.intArr[1] = m_height;
	m_info.intArr[2] = m_brushIndex;
	m_info.intArr[3] = m_weightIndex;

	m_info.vec2Arr[0] = m_brushScale; // scale value

	g_materialBuffer->SetData(&m_info);
	g_materialBuffer->UpdateData(SHADER_STAGE::Compute);

	m_weightMap->UpdateDataRW(0);
	m_brushArrTex->UpdateData(SHADER_STAGE::Compute, (UINT)SHADER_PARAM::TEXTURE_ARR_0 - (UINT)SHADER_PARAM::TEXTURE_0);

	m_raycastInfo->UpdateData(13, SHADER_STAGE::Compute);
	m_inputIndex->UpdateData(14, SHADER_STAGE::Compute);
}

void CWeightMapShader::Clear()
{
	g_materialBuffer->Clear(SHADER_STAGE::Compute);

	if (nullptr != m_weightMap)
	{
		m_weightMap->ClearRW();
		m_weightMap = nullptr;
	}

	if (nullptr != m_brushArrTex)
	{
		m_brushArrTex->Clear(0);
		m_brushArrTex = nullptr;
	}

	if (nullptr != m_raycastInfo)
	{
		m_raycastInfo->Clear();
		m_raycastInfo = nullptr;
	}

	if (nullptr != m_inputIndex)
	{
		m_inputIndex->Clear();
		m_inputIndex = nullptr;
	}
}

void CWeightMapShader::Excute()
{
	if (nullptr == m_weightMap)
		return;

	UINT groupX = (m_width / GetThreadX()) + 1;
	UINT groupY = (m_height / GetThreadY()) + 1;

	Dispatch(groupX, groupY, 1);
}
