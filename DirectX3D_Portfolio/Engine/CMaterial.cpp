#include "pch.h"
#include "CMaterial.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"

CMaterial::CMaterial() :
	CRes(RESOURCE_TYPE::MATERIAL),
	m_shader(nullptr),
	m_param{},
	m_arrTexture{}
{
}

CMaterial::CMaterial(bool _bDefault) :
	CRes(RESOURCE_TYPE::MATERIAL, _bDefault),
	m_shader(nullptr),
	m_param{},
	m_arrTexture{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetData(SHADER_PARAM _param, void* _data)
{
	switch (_param)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		m_param.intArr[(UINT)_param - (UINT)SHADER_PARAM::INT_0] = *((int*)_data);
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		m_param.floatArr[(UINT)_param - (UINT)SHADER_PARAM::FLOAT_0] = *((float*)_data);
		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		m_param.vec2Arr[(UINT)_param - (UINT)SHADER_PARAM::VEC2_0] = *((Vec2*)_data);
		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		m_param.vec4Arr[(UINT)_param - (UINT)SHADER_PARAM::VEC4_0] = *((Vec4*)_data);
		break;
	case SHADER_PARAM::MATRIX_0:
	case SHADER_PARAM::MATRIX_1:
	case SHADER_PARAM::MATRIX_2:
	case SHADER_PARAM::MATRIX_3:
		m_param.matrixArr[(UINT)_param - (UINT)SHADER_PARAM::MATRIX_0] = *((Matrix*)_data);
		break;
	case SHADER_PARAM::TEXTURE_0:
	case SHADER_PARAM::TEXTURE_1:
	case SHADER_PARAM::TEXTURE_2:
	case SHADER_PARAM::TEXTURE_3:
	case SHADER_PARAM::TEXTURE_4:
	case SHADER_PARAM::TEXTURE_5:
	case SHADER_PARAM::TEXTURE_6:
	case SHADER_PARAM::TEXTURE_7:
	case SHADER_PARAM::TEXTURE_ARR_0:
	case SHADER_PARAM::TEXTURE_ARR_1:
	case SHADER_PARAM::TEXTURE_CUBE_0:
	case SHADER_PARAM::TEXTURE_CUBE_1:
		m_arrTexture[(UINT)_param - (UINT)SHADER_PARAM::TEXTURE_0] = (CTexture*)_data;

		if(nullptr != _data)
			m_param.bTex[(UINT)_param - (UINT)SHADER_PARAM::TEXTURE_0] = true;
		else
			m_param.bTex[(UINT)_param - (UINT)SHADER_PARAM::TEXTURE_0] = false;
		break;
	}

	if (!IsDefault() && SCENE_MODE::STOP == CSceneManager::GetInst()->GetSceneMode())
	{
		//Save(GetRelativePath());
	}
}

void CMaterial::GetData(SHADER_PARAM _param, void* _out)
{
	switch (_param)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		(*(int*)_out) = m_param.intArr[(UINT)_param - (UINT)SHADER_PARAM::INT_0];
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		(*(float*)_out) = m_param.floatArr[(UINT)_param - (UINT)SHADER_PARAM::FLOAT_0];
		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		(*(Vec2*)_out) = m_param.vec2Arr[(UINT)_param - (UINT)SHADER_PARAM::VEC2_0];
		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		(*(Vec4*)_out) = m_param.vec4Arr[(UINT)_param - (UINT)SHADER_PARAM::VEC4_0];
		break;
	case SHADER_PARAM::MATRIX_0:
	case SHADER_PARAM::MATRIX_1:
	case SHADER_PARAM::MATRIX_2:
	case SHADER_PARAM::MATRIX_3:
		(*(Matrix*)_out) = m_param.matrixArr[(UINT)_param - (UINT)SHADER_PARAM::MATRIX_0];
		break;
	case SHADER_PARAM::TEXTURE_0:
	case SHADER_PARAM::TEXTURE_1:
	case SHADER_PARAM::TEXTURE_2:
	case SHADER_PARAM::TEXTURE_3:
	case SHADER_PARAM::TEXTURE_4:
	case SHADER_PARAM::TEXTURE_5:
	case SHADER_PARAM::TEXTURE_6:
	case SHADER_PARAM::TEXTURE_7:
	case SHADER_PARAM::TEXTURE_ARR_0:
	case SHADER_PARAM::TEXTURE_ARR_1:
	case SHADER_PARAM::TEXTURE_CUBE_0:
	case SHADER_PARAM::TEXTURE_CUBE_1:
		(*(CTexture**)_out) = m_arrTexture[(UINT)_param - (UINT)SHADER_PARAM::TEXTURE_0].Get();
		break;
	}
}

void CMaterial::UpdateData()
{
	m_shader->UpdateData();

	for (UINT i = 0; i < TEXTURE_COUNT; ++i)
	{
		if (nullptr != m_arrTexture[i])
		{
			m_arrTexture[i]->UpdateData(SHADER_STAGE::ALL, i);
			m_param.bTex[i] = true;
		}
		else
		{
			m_param.bTex[i] = false;
		}
	}

	static const CConstBuffer* CB = CDevice::GetInst()->GetCB(CONST_BUFFER::MATERIAL_PARAM);
	CB->SetData(&m_param);
	CB->UpdateData();
}

void CMaterial::UpdateData_Inst()
{
	m_shader->UpdateData_Inst();

	for (UINT i = 0; i < TEXTURE_COUNT; ++i)
	{
		if (nullptr != m_arrTexture[i])
		{
			m_arrTexture[i]->UpdateData(SHADER_STAGE::ALL, i);
			m_param.bTex[i] = true;
		}
		else
		{
			m_param.bTex[i] = false;
		}
	}

	static const CConstBuffer* CB = CDevice::GetInst()->GetCB(CONST_BUFFER::MATERIAL_PARAM);
	CB->SetData(&m_param);
	CB->UpdateData();
}

void CMaterial::Save(const wstring& _relativePath)
{
	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _relativePath;

	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, filePath.c_str(), L"wb");

	assert(file);

	SaveResourceToFile(m_shader, file);
	SaveToFile(&m_param, file);

	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0; ++i)
	{
		SaveResourceToFile(m_arrTexture[i], file);
	}

	fclose(file);
}

int CMaterial::Load(const wstring& _filePath)
{
	FILE* file = nullptr;
	_wfopen_s(&file, _filePath.c_str(), L"rb");

	assert(file);

	LoadResourceFromFile(m_shader, file);
	LoadFromFile(&m_param, file);

	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0; ++i)
	{
		LoadResourceFromFile(m_arrTexture[i], file);
	}

	fclose(file);

	return S_OK;
}

void CMaterial::Clear()
{
	CGraphicsShader::Clear();

	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0; ++i)
	{
		CTexture::Clear(i);
	}
}

CMaterial* CMaterial::Clone()
{
	Ptr<CMaterial> material = new CMaterial(*this);
	CResourceManager::GetInst()->AddCloneMaterial(material);

	return material.Get();
}