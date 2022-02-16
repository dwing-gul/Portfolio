#include "pch.h"
#include "CHeightMapShader.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CHeightMapShader::CHeightMapShader() :
	CComputeShader(32, 32, 1),
	m_input(nullptr),
	m_inputIndex(nullptr),
	m_brushIndex(0),
	m_mode(0)
{
}

CHeightMapShader::~CHeightMapShader()
{
}

void CHeightMapShader::UpdateData()
{
	m_info.intArr[0] = (int)m_heightMap->Width();
	m_info.intArr[1] = (int)m_heightMap->Height();
	m_info.intArr[2] = m_brushIndex;
	m_info.intArr[3] = m_mode;

	m_info.vec2Arr[0] = m_scale; // scale value

	g_materialBuffer->SetData(&m_info);
	g_materialBuffer->UpdateData(SHADER_STAGE::Compute);

	m_heightMap->UpdateRWData(0);
	m_brushTex->UpdateData(SHADER_STAGE::Compute, (UINT)SHADER_PARAM::TEXTURE_ARR_0 - (UINT)SHADER_PARAM::TEXTURE_0);

	m_input->UpdateData(13, SHADER_STAGE::Compute);
	m_inputIndex->UpdateData(14, SHADER_STAGE::Compute);
}

void CHeightMapShader::Clear()
{
	g_materialBuffer->Clear(SHADER_STAGE::Compute);

	if (nullptr != m_heightMap)
	{
		m_heightMap->ClearRW(0);
		m_heightMap = nullptr;
	}

	if (nullptr != m_brushTex)
	{
		m_brushTex->Clear(0);
		m_brushTex = nullptr;
	}

	if (nullptr != m_input)
	{
		m_input->Clear();
		m_input = nullptr;
	}

	if (nullptr != m_inputIndex)
	{
		m_inputIndex->Clear();
		m_inputIndex = nullptr;
	}
}

void CHeightMapShader::Excute()
{
	if (nullptr == m_heightMap)
		return;

	UINT groupX = ((UINT)m_heightMap->Width() / GetThreadX()) + 1;
	UINT groupY = ((UINT)m_heightMap->Height() / GetThreadY()) + 1;

	Dispatch(groupX, groupY, 1);
}
