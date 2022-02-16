#include "pch.h"
#include "StructuredBuffer.h"
#include "Device.h"

StructuredBuffer::StructuredBuffer() :
	m_desc{},
	m_elementSize(0),
	m_elementCount(0),
	m_recentRegisterNum(0),
	m_type(STRUCTURED_TYPE::READONLY)
{
}

StructuredBuffer::~StructuredBuffer()
{
}

void StructuredBuffer::Create(STRUCTURED_TYPE _type, UINT _elementSize, UINT _elementCount, void* _systemData)
{
	m_type = _type;
	m_SB_R = nullptr;
	m_SB_RW = nullptr;
	m_shaderResourceView = nullptr;
	m_unorderedAccessView = nullptr;
	m_elementSize = _elementSize;
	m_elementCount = _elementCount;

	m_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_desc.ByteWidth = m_elementSize * m_elementCount;
	m_desc.StructureByteStride = m_elementSize;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.BufferEx.NumElements = m_elementCount;

	if (STRUCTURED_TYPE::READONLY == m_type)
	{
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_desc.Usage = D3D11_USAGE_DYNAMIC;
		m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (nullptr != _systemData)
		{
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = _systemData;
			DEVICE->CreateBuffer(&m_desc, &sub, m_SB_R.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc, nullptr, m_SB_R.GetAddressOf());
		}

		DEVICE->CreateShaderResourceView(m_SB_R.Get(), &desc, m_shaderResourceView.GetAddressOf());
		assert(m_shaderResourceView.Get());
	}
	else if (STRUCTURED_TYPE::READ_WRITE == m_type)
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

		DEVICE->CreateShaderResourceView(m_SB_RW.Get(), &desc, m_shaderResourceView.GetAddressOf());
		assert(m_shaderResourceView.Get());

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = m_elementCount;

		DEVICE->CreateUnorderedAccessView(m_SB_RW.Get(), &UAVDesc, m_unorderedAccessView.GetAddressOf());
		assert(m_unorderedAccessView.Get());
	}
	else
	{
		m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_desc.Usage = D3D11_USAGE_DYNAMIC;
		m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (nullptr != _systemData)
		{
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = _systemData;
			DEVICE->CreateBuffer(&m_desc, &sub, m_SB_R.GetAddressOf());
		}
		else
		{
			DEVICE->CreateBuffer(&m_desc, nullptr, m_SB_R.GetAddressOf());
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

		DEVICE->CreateShaderResourceView(m_SB_RW.Get(), &desc, m_shaderResourceView.GetAddressOf());
		assert(m_shaderResourceView.Get());

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = m_elementCount;

		DEVICE->CreateUnorderedAccessView(m_SB_RW.Get(), &UAVDesc, m_unorderedAccessView.GetAddressOf());
		assert(m_unorderedAccessView.Get());
	}
}

void StructuredBuffer::SetData(void* _sysMem, UINT _elementCount) const
{
	if (STRUCTURED_TYPE::READ_WRITE == m_type)
		assert(nullptr);

	int size = m_elementSize * _elementCount;

	D3D11_MAPPED_SUBRESOURCE sub = {};
	CONTEXT->Map(m_SB_R.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, _sysMem, size);
	CONTEXT->Unmap(m_SB_R.Get(), 0);

	if (STRUCTURED_TYPE::DUAL == m_type)
	{
		CONTEXT->CopyResource(m_SB_RW.Get(), m_SB_R.Get());
	}
}

void StructuredBuffer::UpdateData(UINT _registerNum, SHADER_STAGE _stage)
{
	m_recentRegisterNum = _registerNum;

	if ((UINT)_stage & (UINT)SHADER_STAGE::Vertex)
		CONTEXT->VSSetShaderResources(m_recentRegisterNum, 1, m_shaderResourceView.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Hull)
		CONTEXT->HSSetShaderResources(m_recentRegisterNum, 1, m_shaderResourceView.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Domain)
		CONTEXT->DSSetShaderResources(m_recentRegisterNum, 1, m_shaderResourceView.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Geometry)
		CONTEXT->GSSetShaderResources(m_recentRegisterNum, 1, m_shaderResourceView.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Pixel)
		CONTEXT->PSSetShaderResources(m_recentRegisterNum, 1, m_shaderResourceView.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Compute)
		CONTEXT->CSSetShaderResources(m_recentRegisterNum, 1, m_shaderResourceView.GetAddressOf());
}

void StructuredBuffer::UpdateDataRW(UINT _registerNum)
{
	m_recentRegisterNum = _registerNum;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_recentRegisterNum, 1, m_unorderedAccessView.GetAddressOf(), &i);
}

void StructuredBuffer::Clear(SHADER_STAGE _stage)
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

void StructuredBuffer::ClearRW()
{
	ID3D11UnorderedAccessView* UAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_recentRegisterNum, 1, &UAV, &i);
}