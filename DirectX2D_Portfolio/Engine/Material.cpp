#include "pch.h"
#include "Material.h"
#include "ConstBuffer.h"
#include "Device.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "PathManager.h"

Material::Material() :
	Res(RESOURCE_TYPE::MATERIAL),
	m_param{},
	m_arrTex{}
{
}

Material::Material(bool _bDefault) :
	Res(RESOURCE_TYPE::MATERIAL, _bDefault),
	m_shader(nullptr),
	m_param{},
	m_arrTex{}
{
}

Material::~Material()
{
}

void Material::SetData(SHADER_PARAM _param, void* _data)
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
	case SHADER_PARAM::VECTOR2_0:
	case SHADER_PARAM::VECTOR2_1:
	case SHADER_PARAM::VECTOR2_2:
	case SHADER_PARAM::VECTOR2_3:
		m_param.vector2Arr[(UINT)_param - (UINT)SHADER_PARAM::VECTOR2_0] = *((Vector2*)_data);
		break;
	case SHADER_PARAM::VECTOR4_0:
	case SHADER_PARAM::VECTOR4_1:
	case SHADER_PARAM::VECTOR4_2:
	case SHADER_PARAM::VECTOR4_3:
		m_param.vector4Arr[(UINT)_param - (UINT)SHADER_PARAM::VECTOR4_0] = *((Vector4*)_data);
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
		m_arrTex[(UINT)_param - (UINT)SHADER_PARAM::TEXTURE_0] = (Texture*)_data;
		break;
	}

	if (!IsDefault() && SCENE_MODE::STOP == SceneManager::GetInst()->GetSceneMode())
	{
		Save(GetRelativePath());
	}
}

void Material::GetData(SHADER_PARAM _param, void* _data)
{
	switch (_param)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		(*(int*)_data) = m_param.intArr[(UINT)_param - (UINT)SHADER_PARAM::INT_0];
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		(*(float*)_data) = m_param.floatArr[(UINT)_param - (UINT)SHADER_PARAM::FLOAT_0];
		break;
	case SHADER_PARAM::VECTOR2_0:
	case SHADER_PARAM::VECTOR2_1:
	case SHADER_PARAM::VECTOR2_2:
	case SHADER_PARAM::VECTOR2_3:
		(*(Vector2*)_data) = m_param.vector2Arr[(UINT)_param - (UINT)SHADER_PARAM::VECTOR2_0];
		break;
	case SHADER_PARAM::VECTOR4_0:
	case SHADER_PARAM::VECTOR4_1:
	case SHADER_PARAM::VECTOR4_2:
	case SHADER_PARAM::VECTOR4_3:
		(*(Vector4*)_data) = m_param.vector4Arr[(UINT)_param - (UINT)SHADER_PARAM::VECTOR4_0];
		break;
	case SHADER_PARAM::MATRIX_0:
	case SHADER_PARAM::MATRIX_1:
	case SHADER_PARAM::MATRIX_2:
	case SHADER_PARAM::MATRIX_3:
		(*(Matrix*)_data) = m_param.matrixArr[(UINT)_param - (UINT)SHADER_PARAM::MATRIX_0];
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
		(*(Texture**)_data) = m_arrTex[(UINT)_param - (UINT)SHADER_PARAM::TEXTURE_0].Get();
		break;
	}
}

void Material::UpdateData()
{
	m_shader->UpdateData();

	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0; ++i)
	{
		if (nullptr != m_arrTex[i])
		{
			m_arrTex[i]->UpdateData(SHADER_STAGE::ALL, i);
			m_param.bTex[i] = true;
		}
		else
		{
			m_param.bTex[i] = false;
		}
	}

	static const ConstBuffer* CB = Device::GetInst()->GetCB(CONST_BUFFER::MATERIAL_PARAM);
	CB->SetData(&m_param);
	CB->UpdateData();
}

void Material::Save(const wstring& _relativePath)
{
	wstring filePath = PathManager::GetInst()->GetContentPath();
	filePath += _relativePath;
	const wchar_t* check = wcsstr(_relativePath.c_str(), L".material");

	if (check == NULL)
	{
		filePath += L".material";
	}

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");

	assert(file);

	SaveWStringToFile(GetKey(), file);
	SaveWStringToFile(GetRelativePath(), file);
	SaveResourceToFile(m_shader, file);
	SaveToFile(&m_param, file);

	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0; ++i)
	{
		SaveResourceToFile(m_arrTex[i], file);
	}

	fclose(file);
}

int Material::Load(const wstring& _filePath)
{
	wstring filePath = _filePath;
	if (wstring::npos == _filePath.find(L".material"))
		filePath += L".material";

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");

	assert(file);

	wstring key;
	LoadWStringFromFile(key, file);
	SetKey(key);

	wstring relativePath;
	LoadWStringFromFile(relativePath, file);
	SetRelativePath(relativePath);

	LoadResourceFromFile(m_shader, file);
	LoadFromFile(&m_param, file);

	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0; ++i)
	{
		LoadResourceFromFile(m_arrTex[i], file);
	}

	fclose(file);

	return S_OK;
}

void Material::Clear()
{
	GraphicsShader::Clear();

	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEXTURE_END - (UINT)SHADER_PARAM::TEXTURE_0; ++i)
	{
		Texture::Clear(i);
	}
}

Material* Material::Clone()
{
	Ptr<Material> material = new Material(*this);
	ResourceManager::GetInst()->AddCloneMaterial(material);

	return material.Get();
}
