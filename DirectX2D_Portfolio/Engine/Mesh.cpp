#include "pch.h"
#include "Mesh.h"
#include "Device.h"

Mesh::Mesh() :
	Res(RESOURCE_TYPE::MESH, true),
	m_vertexSys(nullptr),
	m_indexSys(nullptr),
	m_vertexDesc{},
	m_indexDesc{},
	m_indexCount(0)
{
}

Mesh::~Mesh()
{
	Safe_Delete(m_vertexSys);
	Safe_Delete(m_indexSys);
}

void Mesh::Create(void* _vertexSys, UINT _vertexBufferSize, void* _indexSys, UINT _indexBufferSize)
{
	m_indexCount = _indexBufferSize / sizeof(UINT);

	m_vertexDesc.ByteWidth = _vertexBufferSize;
	m_vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	m_vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_vertexDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = _vertexSys;

	if (FAILED(DEVICE->CreateBuffer(&m_vertexDesc, &sub, m_vertexBuffer.GetAddressOf())))
	{
		MessageBox(nullptr, L"버퍼 생성 실패", L"메쉬 생성 실패", MB_OK);
		return;
	}

	m_indexDesc.ByteWidth = _indexBufferSize;
	m_indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	m_indexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_indexDesc.CPUAccessFlags = 0;

	sub = {};
	sub.pSysMem = _indexSys;

	if(FAILED(DEVICE->CreateBuffer(&m_indexDesc, &sub, m_indexBuffer.GetAddressOf())))
	{
		MessageBox(nullptr, L"버퍼 생성 실패", L"메쉬 생성 실패", MB_OK);
		return;
	}

	m_vertexSys = malloc(_vertexBufferSize);
	memcpy(m_vertexSys, _vertexSys, _vertexBufferSize);

	m_indexSys = malloc(_indexBufferSize);
	memcpy(m_indexSys, _indexSys, _indexBufferSize);
}

void Mesh::UpdateData()
{
	UINT stride = sizeof(tVertex);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::render()
{
	CONTEXT->DrawIndexed(m_indexCount, 0, 0);
}

void Mesh::render_particle(UINT _count)
{
	CONTEXT->DrawIndexedInstanced(m_indexCount, _count, 0, 0, 0);
}
