#include "pch.h"
#include "ComputeShader.h"
#include "PathManager.h"
#include "Device.h"
#include "ConstBuffer.h"

const ConstBuffer* ComputeShader::g_materialBuffer = nullptr;

ComputeShader::ComputeShader(UINT _x, UINT _y, UINT _z) :
	Shader(RESOURCE_TYPE::COMPUTE),
	m_info{},
	m_threadNumX(_x),
	m_threadNumY(_y),
	m_threadNumZ(_z)
{
	if (nullptr == g_materialBuffer)
	{
		g_materialBuffer = Device::GetInst()->GetCB(CONST_BUFFER::MATERIAL_PARAM);
	}
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::CreateComputeShader(const wstring& _relativePath, const string& _funcName)
{
	wstring shaderFile = PathManager::GetInst()->GetContentPath();
	shaderFile += _relativePath;

	int flag = D3DCOMPILE_DEBUG;

	if (FAILED(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _funcName.c_str(), "cs_5_0", flag, 0, m_CSBlob.GetAddressOf(),
		m_ErrorBlob.GetAddressOf())))
	{
		char* ErrMsg = (char*)m_ErrorBlob->GetBufferPointer();
		MessageBoxA(nullptr, ErrMsg, "Compute Shader Create Fail", MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(), nullptr, m_CS.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void ComputeShader::Dispatch(UINT _x, UINT _y, UINT _z)
{
	UpdateData();

	CONTEXT->CSSetShader(m_CS.Get(), 0, 0);
	CONTEXT->Dispatch(_x, _y, _z);

	Clear();
}
