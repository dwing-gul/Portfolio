#include "pch.h"
#include "CAnimation3DShader.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CAnimation3DShader::CAnimation3DShader() :
	CComputeShader(256, 1, 1),
	m_info{},
	m_FrameDataBuffer(nullptr),
	m_OffsetMatrixBuffer(nullptr),
	m_OutputBuffer(nullptr),
	m_BoneMatrixBuffer(nullptr)
{
}

CAnimation3DShader::~CAnimation3DShader()
{
}

void CAnimation3DShader::UpdateData()
{
	// 필요한 상수값 전달	
	static const CConstBuffer* materialBuffer = CDevice::GetInst()->GetCB(CONST_BUFFER::MATERIAL_PARAM);
	materialBuffer->SetData(&m_info);
	materialBuffer->UpdateData(SHADER_STAGE::Compute);

	// 구조화버퍼 전달
	m_FrameDataBuffer->UpdateData(13);
	m_OffsetMatrixBuffer->UpdateData(14);
	
	m_OutputBuffer->UpdateDataRW(0);
	if (nullptr != m_BoneMatrixBuffer)
	{
		m_BoneMatrixBuffer->UpdateDataRW(1);
	}
}

void CAnimation3DShader::Excute()
{
	UINT iGroupX = (m_info.intArr[0] / GetThreadX()) + 1;
	Dispatch(iGroupX, 1, 1);
}

void CAnimation3DShader::Clear()
{
	// 전달한 구조화버퍼 클리어	
	m_FrameDataBuffer->Clear();
	m_OffsetMatrixBuffer->Clear();
	m_OutputBuffer->ClearRW();
	
	if (nullptr != m_BoneMatrixBuffer)
	{
		m_BoneMatrixBuffer->ClearRW();
		m_BoneMatrixBuffer = nullptr;
	}
}