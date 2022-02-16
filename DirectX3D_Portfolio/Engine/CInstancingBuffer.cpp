#include "pch.h"
#include "CInstancingBuffer.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"
#include "CCopyBoneMatrixShader.h"
#include "Ptr.h"
#include "CMaterial.h"
#include "CResourceManager.h"

CInstancingBuffer::CInstancingBuffer()
	: m_maxCount(10),
	m_animInstancingCount(0),
	m_boneBuffer(nullptr)
{
	m_boneBuffer = new CStructuredBuffer;
}

CInstancingBuffer::~CInstancingBuffer()
{
	Safe_Delete_Ptr(m_boneBuffer);
}

void CInstancingBuffer::init()
{
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = sizeof(tInstancingData) * m_maxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_instancingBuffer)))
		assert(NULL);

	m_copyShader = (CCopyBoneMatrixShader*)CResourceManager::GetInst()->FindRes<CComputeShader>(L"CopyBoneMatrixShader").Get();
}

void CInstancingBuffer::SetData()
{
	if (m_vecData.size() > m_maxCount)
	{
		Resize((UINT)m_vecData.size());
	}

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(m_instancingBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	memcpy(tMap.pData, &m_vecData[0], sizeof(tInstancingData) * m_vecData.size());
	CONTEXT->Unmap(m_instancingBuffer.Get(), 0);

	// 본 행렬정보 메모리 복사
	if (m_vecBoneMatrix.empty())
		return;

	UINT bufferSize = (UINT)m_vecBoneMatrix.size() * m_vecBoneMatrix[0]->GetBufferSize();
	if (m_boneBuffer->GetBufferSize() < bufferSize)
	{
		m_boneBuffer->Create(STRUCTURED_TYPE::READ_WRITE, m_vecBoneMatrix[0]->GetElementSize(), m_vecBoneMatrix[0]->GetElementCount() * (UINT)m_vecBoneMatrix.size(), nullptr);
	}

	//// 복사용 컴퓨트 쉐이더 실행
	UINT boneCount = m_vecBoneMatrix[0]->GetElementCount();
	m_copyShader->SetBoneCount(boneCount);

	for (UINT i = 0; i < (UINT)m_vecBoneMatrix.size(); ++i)
	{
		m_copyShader->SetRowIndex(i);
		m_copyShader->SetSourceBuffer(m_vecBoneMatrix[i]);
		m_copyShader->SetDestBuffer(m_boneBuffer);
		m_copyShader->Excute();
	}

	m_boneBuffer->UpdateData(12);
}

void CInstancingBuffer::AddInstancingBoneMatrix(CStructuredBuffer* _buffer)
{
	++m_animInstancingCount;
	m_vecBoneMatrix.push_back(_buffer);
}

void CInstancingBuffer::Resize(UINT _iCount)
{
	m_instancingBuffer = nullptr;

	m_maxCount = _iCount;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = sizeof(tInstancingData) * m_maxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_instancingBuffer)))
		assert(NULL);
}