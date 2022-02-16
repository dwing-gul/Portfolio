#pragma once
#include "Ptr.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMeshData.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CPrefab.h"
#include "CSound.h"
#include "CMesh.h"
#include "CPathManager.h"

class CResourceManager :
	public CSingleton<CResourceManager>
{
	SINGLE(CResourceManager);
private :
	map<wstring, CRes*> m_mapRes[(UINT)RESOURCE_TYPE::END];
	vector<CMaterial*> m_vecCloneMaterial;

	vector<CMeshData*> m_vecMeshData;
	bool m_bEvent;

public :
	void init();
	void update() { m_bEvent = false; }

private :
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateComputeShader();
	void InitSound();

public :
	template <typename T>
	Ptr<T> Load(const wstring& _key, const wstring& _relativePath, RENDER_POV _POV = RENDER_POV::FORWARD);

	template <typename T>
	void AddRes(const wstring& _key, T* _resource);

	void AddRes(const wstring& _key, CRes* _res, RESOURCE_TYPE _type);

	void SubRes(const wstring& _key, RESOURCE_TYPE _type);

	template <typename T>
	Ptr<T> FindRes(const wstring& _key);

	void AddCloneMaterial(Ptr<CMaterial> _material) { m_vecCloneMaterial.push_back(_material.Get()); }

	Ptr<CTexture> CreateTexture(const wstring& _key, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
	Ptr<CTexture> CreateTexture_Tool(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
	Ptr<CTexture> CreateTexture(const wstring& _key, ComPtr<ID3D11Texture2D> _texture);

	Ptr<CTexture> LoadTexture(const wstring& _key, const wstring& _relativePath, int _mipLevel);

	Ptr<CMeshData> LoadFBX(const wstring& _path, bool _bHasAnim = false);
	vector<CMeshData*>& LoadFBX_vector(const wstring& _path, bool _bHasAnim = false);

	void GetResName(RESOURCE_TYPE _type, vector<wstring>& _output);
	const map<wstring, CRes*>& GetRes(RESOURCE_TYPE _type) { return m_mapRes[(UINT)_type]; }

	template <typename T>
	void FixOverlapName(wstring& _name, T* _res);

	bool IsHappened() { return m_bEvent; }
};

template<typename T>
RESOURCE_TYPE GetResourceType()
{
	const type_info& info = typeid(T);
	const type_info& meshdata = typeid(CMeshData);
	const type_info& prefab = typeid(CPrefab);
	const type_info& material = typeid(CMaterial);
	const type_info& mesh = typeid(CMesh);
	const type_info& graphics = typeid(CGraphicsShader);
	const type_info& compute = typeid(CComputeShader);
	const type_info& texture = typeid(CTexture);
	const type_info& sound = typeid(CSound);

	if (&info == &meshdata)
		return RESOURCE_TYPE::MESHDATA;
	else if (&info == &prefab)
		return RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		return RESOURCE_TYPE::MATERIAL;
	else if (&info == &mesh)
		return RESOURCE_TYPE::MESH;
	else if (&info == &graphics)
		return RESOURCE_TYPE::GRAPHICS;
	else if (&info == &compute)
		return RESOURCE_TYPE::COMPUTE;
	else if (&info == &texture)
		return RESOURCE_TYPE::TEXTURE;
	else if (&info == &sound)
		return RESOURCE_TYPE::SOUND;

	return RESOURCE_TYPE::END;
}

template<typename T>
Ptr<T> CResourceManager::Load(const wstring& _key, const wstring& _relativePath, RENDER_POV _POV)
{
	RESOURCE_TYPE type = GetResourceType<T>();

	CRes* res = FindRes<T>(_key).Get();
	if (nullptr != res)
	{
		return (T*)res;
	}

	if (RESOURCE_TYPE::GRAPHICS == type)
	{
		res = new CGraphicsShader(_POV);
	}
	else
	{
		res = new T;
	}

	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _relativePath;

	if (FAILED(res->Load(filePath)))
	{
		return nullptr;
	}

	res->SetKey(_key);
	res->SetRelativePath(_relativePath);
	AddRes<T>(_key, (T*)res);

	m_bEvent = true;

	return (T*)res;
}

template<typename T>
void CResourceManager::AddRes(const wstring& _key, T* _resource)
{
	static int overlapKey = 1;
	RESOURCE_TYPE type = GetResourceType<T>();

	CRes* res = FindRes<T>(_key).Get();
	if (nullptr != res)
	{
		if (RESOURCE_TYPE::PREFAB == type)
		{
			m_mapRes[(UINT)type].erase(res->m_key);
			delete res;
			res = nullptr;
		}
		else
		{
			wchar_t attach[255] = L"-";
			swprintf_s(attach, L"%d", overlapKey++);
			wstring key = _key;
			key += attach;

			AddRes<T>(key, _resource);
		}
	}
	else
	{
		overlapKey = 1;
	}

	_resource->SetKey(_key);
	m_mapRes[(UINT)type].insert(make_pair(_key, _resource));
	m_bEvent = true;
}

template<typename T>
Ptr<T> CResourceManager::FindRes(const wstring& _key)
{
	RESOURCE_TYPE type = GetResourceType<T>();

	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)type].find(_key);

	if (iter == m_mapRes[(UINT)type].end())
		return nullptr;

	return (T*)iter->second;
}

template<typename T>
void CResourceManager::FixOverlapName(wstring& _name, T* _res)
{
	static int overlapID = 1;

	if (nullptr != CResourceManager::GetInst()->FindRes<T>(_name))
	{
		_name = _res->GetName();

		wchar_t attach[255] = L"";
		swprintf_s(attach, L"-%d", overlapID++);
		_name += attach;

		FixOverlapName(_name, _res);
	}
	else
	{
		overlapID = 1;
		return;
	}
}


