#include "pch.h"
#include "ConstBuffer.h"
#include "Device.h"

ConstBuffer::ConstBuffer() :
	m_desc{},
	m_type(CONST_BUFFER::END)
{
}

ConstBuffer::~ConstBuffer()
{
}

void ConstBuffer::Create(CONST_BUFFER _type, UINT _bufferSize)
{
	m_type = _type;

	m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_desc.Usage = D3D11_USAGE_DYNAMIC;
	m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_desc.ByteWidth = _bufferSize;

	DEVICE->CreateBuffer(&m_desc, nullptr, m_CB.GetAddressOf());
}

void ConstBuffer::SetData(void* _sysMem) const
{
	D3D11_MAPPED_SUBRESOURCE sub = {};
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, _sysMem, m_desc.ByteWidth);
	CONTEXT->Unmap(m_CB.Get(), 0);
}

void ConstBuffer::SetData(void* _sysMem, UINT _size) const
{
	D3D11_MAPPED_SUBRESOURCE sub = {};
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, _sysMem, _size);
	CONTEXT->Unmap(m_CB.Get(), 0);
}

void ConstBuffer::UpdateData(SHADER_STAGE _stage) const
{
	if ((UINT)_stage & (UINT)SHADER_STAGE::Vertex)
		CONTEXT->VSSetConstantBuffers((UINT)m_type, 1, m_CB.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Hull)
		CONTEXT->HSSetConstantBuffers((UINT)m_type, 1, m_CB.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Domain)
		CONTEXT->DSSetConstantBuffers((UINT)m_type, 1, m_CB.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Geometry)
		CONTEXT->GSSetConstantBuffers((UINT)m_type, 1, m_CB.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Pixel)
		CONTEXT->PSSetConstantBuffers((UINT)m_type, 1, m_CB.GetAddressOf());
	if ((UINT)_stage & (UINT)SHADER_STAGE::Compute)
		CONTEXT->CSSetConstantBuffers((UINT)m_type, 1, m_CB.GetAddressOf());
}

void ConstBuffer::Clear(SHADER_STAGE _stage) const
{
	ID3D11Buffer* buffer = nullptr;
	if ((UINT)_stage & (UINT)SHADER_STAGE::Vertex)
		CONTEXT->VSSetConstantBuffers((UINT)m_type, 1, &buffer);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Hull)
		CONTEXT->HSSetConstantBuffers((UINT)m_type, 1, &buffer);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Domain)
		CONTEXT->DSSetConstantBuffers((UINT)m_type, 1, &buffer);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Geometry)
		CONTEXT->GSSetConstantBuffers((UINT)m_type, 1, &buffer);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Pixel)
		CONTEXT->PSSetConstantBuffers((UINT)m_type, 1, &buffer);
	if ((UINT)_stage & (UINT)SHADER_STAGE::Compute)
		CONTEXT->CSSetConstantBuffers((UINT)m_type, 1, &buffer);
}
