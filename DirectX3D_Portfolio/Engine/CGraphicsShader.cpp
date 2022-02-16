#include "pch.h"
#include "CGraphicsShader.h"
#include "CPathManager.h"
#include "CDevice.h"

CGraphicsShader::CGraphicsShader(RENDER_POV _POV) :
	CShader(RESOURCE_TYPE::GRAPHICS),
	m_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_POV(_POV),
	m_DSType(DEPTH_STENCIL_TYPE::LESS),
	m_RSType(RASTERIZER_TYPE::CULL_BACK),
	m_BlendType(BLEND_TYPE::END)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

void CGraphicsShader::CreateVertexShader(const wstring& _relativePath, const string& _funcName)
{
	wstring shaderFile = CPathManager::GetInst()->GetContentPath();
	shaderFile += _relativePath;

	int flag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _funcName.c_str(), "vs_5_0", flag, 0, m_VSBlob.GetAddressOf(),
		m_ErrorBlob.GetAddressOf())))
	{
		char* ErrMsg = (char*)m_ErrorBlob->GetBufferPointer();
		MessageBoxA(nullptr, ErrMsg, "Vertex Shader Create Fail", MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf())))
	{
		assert(nullptr);
	}

	// Instancing ¿ë VertexShader Create
	{
		string VSInst = _funcName;
		VSInst += "_Inst";

		HRESULT hr = D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, VSInst.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0, m_VSInstBlob.GetAddressOf(), m_ErrorBlob.GetAddressOf());

		if (FAILED(hr))
		{
			//MessageBoxA(nullptr, (char*)m_ErrorBlob->GetBufferPointer(), "Shader Compile Error !!", MB_OK);
		}
		else
		{
			DEVICE->CreateVertexShader(m_VSInstBlob->GetBufferPointer(), m_VSInstBlob->GetBufferSize(), nullptr, m_VSInst.GetAddressOf());
		}
	}

	UINT elementCount = sizeof(g_LayoutDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	if (m_VSInstBlob.Get())
	{
		DEVICE->CreateInputLayout(g_LayoutDesc, elementCount, m_VSInstBlob->GetBufferPointer(), 
			m_VSInstBlob->GetBufferSize(), m_Layout.GetAddressOf());
	}
	else
	{
		if (FAILED(DEVICE->CreateInputLayout(g_LayoutDesc, elementCount, m_VSBlob->GetBufferPointer(),
			m_VSBlob->GetBufferSize(), m_Layout.GetAddressOf())))
		{
			assert(nullptr);
		}
	}
}

void CGraphicsShader::CreateHullShader(const wstring& _relativePath, const string& _funcName)
{
	wstring shaderFile = CPathManager::GetInst()->GetContentPath();
	shaderFile += _relativePath;

	int flag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _funcName.c_str(), "hs_5_0", flag, 0, m_HSBlob.GetAddressOf(),
		m_ErrorBlob.GetAddressOf())))
	{
		char* ErrMsg = (char*)m_ErrorBlob->GetBufferPointer();
		MessageBoxA(nullptr, ErrMsg, "Hull Shader Create Fail", MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateHullShader(m_HSBlob->GetBufferPointer(), m_HSBlob->GetBufferSize(), nullptr, m_HS.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CGraphicsShader::CreateDomainShader(const wstring& _relativePath, const string& _funcName)
{
	wstring shaderFile = CPathManager::GetInst()->GetContentPath();
	shaderFile += _relativePath;

	int flag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _funcName.c_str(), "ds_5_0", flag, 0, m_DSBlob.GetAddressOf(),
		m_ErrorBlob.GetAddressOf())))
	{
		char* ErrMsg = (char*)m_ErrorBlob->GetBufferPointer();
		MessageBoxA(nullptr, ErrMsg, "Domain Shader Create Fail", MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateDomainShader(m_DSBlob->GetBufferPointer(), m_DSBlob->GetBufferSize(), nullptr, m_DS.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CGraphicsShader::CreateGeometryShader(const wstring& _relativePath, const string& _funcName)
{
	wstring shaderFile = CPathManager::GetInst()->GetContentPath();
	shaderFile += _relativePath;

	int flag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _funcName.c_str(), "gs_5_0", flag, 0, m_GSBlob.GetAddressOf(),
		m_ErrorBlob.GetAddressOf())))
	{
		char* ErrMsg = (char*)m_ErrorBlob->GetBufferPointer();
		MessageBoxA(nullptr, ErrMsg, "Geometry Shader Create Fail", MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CGraphicsShader::CreatePixelShader(const wstring& _relativePath, const string& _funcName)
{
	wstring shaderFile = CPathManager::GetInst()->GetContentPath();
	shaderFile += _relativePath;

	int flag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _funcName.c_str(), "ps_5_0", flag, 0, m_PSBlob.GetAddressOf(),
		m_ErrorBlob.GetAddressOf())))
	{
		char* ErrMsg = (char*)m_ErrorBlob->GetBufferPointer();
		MessageBoxA(nullptr, ErrMsg, "Pixel Shader Create Fail", MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CGraphicsShader::UpdateData()
{
	CONTEXT->IASetPrimitiveTopology(m_topology);
	CONTEXT->IASetInputLayout(m_Layout.Get());

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	CDevice::GetInst()->SetRS(m_RSType);
	CDevice::GetInst()->SetBS(m_BlendType);
	CDevice::GetInst()->SetDS(m_DSType);
}

void CGraphicsShader::UpdateData_Inst()
{
	CONTEXT->IASetPrimitiveTopology(m_topology);
	CONTEXT->IASetInputLayout(m_Layout.Get());

	CONTEXT->VSSetShader(m_VSInst.Get(), 0, 0);
	CONTEXT->HSSetShader(m_HS.Get(), 0, 0);
	CONTEXT->DSSetShader(m_DS.Get(), 0, 0);
	CONTEXT->GSSetShader(m_GS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);

	CDevice::GetInst()->SetRS(m_RSType);
	CDevice::GetInst()->SetBS(m_BlendType);
	CDevice::GetInst()->SetDS(m_DSType);
}

void CGraphicsShader::Clear()
{
	ID3D11VertexShader* VS = nullptr;
	ID3D11HullShader* HS = nullptr;
	ID3D11DomainShader* DS = nullptr;
	ID3D11GeometryShader* GS = nullptr;
	ID3D11PixelShader* PS = nullptr;

	CONTEXT->VSSetShader(VS, nullptr, 0);
	CONTEXT->HSSetShader(HS, nullptr, 0);
	CONTEXT->DSSetShader(DS, nullptr, 0);
	CONTEXT->GSSetShader(GS, nullptr, 0);
	CONTEXT->PSSetShader(PS, nullptr, 0);
}