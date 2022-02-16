#include "pch.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer() :
	m_desc{},
	m_elementCount(0),
	m_elementSize(0),
	m_recentRegisterNum(0),
	m_type(STRUCTURED_TYPE::READONLY)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

void CStructuredBuffer::Create(STRUCTURED_TYPE _type, UINT _elementSize, UINT _elementCount, void* _systemData)
{
	m_type = _type;
	m_SB_CPU_Read = nullptr;
	m_SB_CPU_Write = nullptr;
	m_SB_RW = nullptr;
	m_SRV = nullptr;
	m_UAV = nullptr;

	m_elementSize = _elementSize;
	m_elementCount = _elementCount;
	m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_desc.ByteWidth = _elementSize * _elementCount;
	m_desc.StructureByteStride = _elementSize;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.BufferEx.NumElements = _elementCount;

	if (STRUCTURED_TYPE::READONLY == _type)
	{
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_desc.Usage = D3D11_USAGE_DYNAMIC;
		m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (nullptr != _systemData)
		{
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = _systemData;
			DEVICE->CreateBuffer(&m_desc, &sub, m_SB_CPU_Write.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc, nullptr, m_SB_CPU_Write.GetAddressOf());
		}

		DEVICE->CreateShaderResourceView(m_SB_CPU_Write.Get(), &desc, m_SRV.GetAddressOf());
		assert(m_SRV.Get());
	}
	else if (STRUCTURED_TYPE::READ_WRITE == _type)
	{
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_desc.Usage = D3D11_USAGE_DEFAULT;
		m_desc.CPUAccessFlags = 0;

		if (nullptr != _systemData)
		{
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = _systemData;
			DEVICE->CreateBuffer(&m_desc, &sub, m_SB_RW.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc, nullptr, m_SB_RW.GetAddressOf());
		}

		DEVICE->CreateShaderResourceView(m_SB_RW.Get(), &desc, m_SRV.GetAddressOf());
		assert(m_SRV.Get());

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = _elementCount;

		DEVICE->CreateUnorderedAccessView(m_SB_RW.Get(), &UAVDesc, m_UAV.GetAddressOf());
		assert(m_UAV.Get());
	}
	else
	{
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_desc.Usage = D3D11_USAGE_DEFAULT;
		m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		if (nullptr != _systemData)
		{
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = _systemData;
			DEVICE->CreateBuffer(&m_desc, &sub, m_SB_CPU_Read.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc, nullptr, m_SB_CPU_Read.GetAddressOf());
		}

		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_desc.Usage = D3D11_USAGE_DYNAMIC;
		m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (nullptr != _systemData)
		{
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = _systemData;
			DEVICE->CreateBuffer(&m_desc, &sub, m_SB_CPU_Write.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc, nullptr, m_SB_CPU_Write.GetAddressOf());
		}

		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_desc.Usage = D3D11_USAGE_DEFAULT;
		m_desc.CPUAccessFlags = 0;

		if (nullptr != _systemData)
		{
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = _systemData;
			DEVICE->CreateBuffer(&m_desc, &sub, m_SB_RW.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc, nullptr, m_SB_RW.GetAddressOf());
		}

		DEVICE->CreateShaderResourceView(m_SB_RW.Get(), &desc, m_SRV.GetAddressOf());
		assert(m_SRV.Get());

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = _elementCount;

		DEVICE->CreateUnorderedAccessView(m_SB_RW.Get(), &UAVDesc, m_UAV.GetAddressOf());
		assert(m_UAV.Get());
	}
}

void CStructuredBuffer::SetData(void* _sysMem, UINT _elementCount) const
{
	if (STRUCTURED_TYPE::READ_WRITE == m_type)
		assert(nullptr);

	int size = m_elementSize * _elementCount;

	D3D11_MAPPED_SUBRESOURCE sub = {};
	CONTEXT->Map(m_SB_CPU_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, _sysMem, size);
	CONTEXT->Unmap(m_SB_CPU_Write.Get(), 0);

	if (STRUCTURED_TYPE::DUAL == m_type)
	{
		CONTEXT->CopyResource(m_SB_RW.Get(), m_SB_CPU_Write.Get());
	}
}

void CStructuredBuffer::GetData(void* _dest) const
{
	if (STRUCTURED_TYPE::READ_WRITE == m_type)
		assert(nullptr);

	if (STRUCTURED_TYPE::DUAL == m_type)
	{
		CONTEXT->CopyResource(m_SB_CPU_Read.Get(), m_SB_RW.Get());
	}

	int size = m_elementSize * m_elementCount;

	D3D11_MAPPED_SUBRESOURCE sub = {};

	CONTEXT->Map(m_SB_CPU_Read.Get(), 0, D3D11_MAP_READ, 0, &sub);
	memcpy(_dest, sub.pData, size);
	CONTEXT->Unmap(m_SB_CPU_Read.Get(), 0);
}

void CStructuredBuffer::UpdateData(UINT _registerNum, SHADER_STAGE _stage)
{
	m_recentRegisterNum = _registerNum;

	if ((UINT)_stage & (UINT)SHADER_STAGE::Vertex)
		CONTEXT->VSSetShaderResources(m_recentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Hull)
		CONTEXT->HSSetShaderResources(m_recentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Domain)
		CONTEXT->DSSetShaderResources(m_recentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Geometry)
		CONTEXT->GSSetShaderResources(m_recentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Pixel)
		CONTEXT->PSSetShaderResources(m_recentRegisterNum, 1, m_SRV.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Compute)
		CONTEXT->CSSetShaderResources(m_recentRegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::UpdateDataRW(UINT _registerNum)
{
	m_recentRegisterNum = _registerNum;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_recentRegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CStructuredBuffer::Clear(SHADER_STAGE _stage)
{
	ID3D11ShaderResourceView* SRV = nullptr;

	if ((UINT)_stage & (UINT)SHADER_STAGE::Vertex)
		CONTEXT->VSSetShaderResources(m_recentRegisterNum, 1, &SRV);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Hull)
		CONTEXT->HSSetShaderResources(m_recentRegisterNum, 1, &SRV);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Domain)
		CONTEXT->DSSetShaderResources(m_recentRegisterNum, 1, &SRV);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Geometry)
		CONTEXT->GSSetShaderResources(m_recentRegisterNum, 1, &SRV);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Pixel)
		CONTEXT->PSSetShaderResources(m_recentRegisterNum, 1, &SRV);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Compute)
		CONTEXT->CSSetShaderResources(m_recentRegisterNum, 1, &SRV);
}

void CStructuredBuffer::ClearRW()
{
	ID3D11UnorderedAccessView* UAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_recentRegisterNum, 1, &UAV, &i);
}
