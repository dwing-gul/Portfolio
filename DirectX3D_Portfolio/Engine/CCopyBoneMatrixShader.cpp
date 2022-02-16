#include "pch.h"
#include "CCopyBoneMatrixShader.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CCopyBoneMatrixShader::CCopyBoneMatrixShader()
	: CComputeShader(1024, 1, 1)
	, m_sourceBuffer(nullptr)
	, m_destBuffer(nullptr)
{
}

CCopyBoneMatrixShader::~CCopyBoneMatrixShader()
{
}

void CCopyBoneMatrixShader::UpdateData()
{
	// 필요한 상수값 전달	
	g_materialBuffer->SetData(&m_info);
	g_materialBuffer->UpdateData(SHADER_STAGE::Compute);

	// 구조화버퍼 전달
	m_sourceBuffer->UpdateData(14, SHADER_STAGE::Compute);
	m_destBuffer->UpdateDataRW(0);
}

void CCopyBoneMatrixShader::Clear()
{
	// 전달한 구조화버퍼 클리어	
	m_sourceBuffer->Clear();
	m_destBuffer->ClearRW();
}

void CCopyBoneMatrixShader::Excute()
{
	int boneCount = m_info.intArr[0];

	UINT groupX = boneCount / 1024 + 1;

	Dispatch(groupX, 1, 1);
}