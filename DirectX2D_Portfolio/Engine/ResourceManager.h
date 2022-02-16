#pragma once
#include "GraphicsShader.h"
#include "ComputeShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Prefab.h"
#include "Sound.h"
#include "PathManager.h"

class ResourceManager :
	public Singleton<ResourceManager>
{
	SINGLE(ResourceManager)
private :
	map<wstring, Res*> m_mapRes[(UINT)RESOURCE_TYPE::END];
	vector<Material*>  m_vectorCloneMaterial;
	bool			   m_bEvent;

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
	Ptr<T> Load(const wstring& _key, const wstring& _relativePath);

	template <typename T>
	void AddRes(const wstring& _key, T* _res);

	void AddRes(const wstring& _key, Res* _res, RESOURCE_TYPE _type);

	void SubRes(const wstring& _key, RESOURCE_TYPE _type);

	template <typename T>
	Ptr<T> FindRes(const wstring& _key);

	void AddCloneMaterial(Ptr<Material> _material) { m_vectorCloneMaterial.push_back(_material.Get()); }

	Ptr<Texture> CreateTexture(const wstring& _key, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
	Ptr<Texture> CreateTexture(const wstring& _key, ComPtr<ID3D11Texture2D> _texture);

	Ptr<Texture> CreateTexture_Changeable(const wstring& _key, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag);
	Ptr<Texture> CreateTexture_Changeable(const wstring& _key, ComPtr<ID3D11Texture2D> _texture);

	void GetResName(RESOURCE_TYPE _type, vector<wstring>& _output);
	const map<wstring, Res*>& GetRes(RESOURCE_TYPE _type) { return m_mapRes[(UINT)_type]; }
	bool IsHappened() { return m_bEvent; }
};

template <typename T>
RESOURCE_TYPE GetResType()
{
	const type_info& info = typeid(T);
	const type_info& prefab = typeid(Prefab);
	const type_info& material = typeid(Material);
	const type_info& mesh = typeid(Mesh);
	const type_info& graphics = typeid(GraphicsShader);
	const type_info& compute = typeid(ComputeShader);
	const type_info& texture = typeid(Texture);
	const type_info& sound = typeid(Sound);

	if (&info == &prefab)
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
	
template <typename T>
inline Ptr<T> ResourceManager::Load(const wstring& _key, const wstring& _relativePath)
{
	RESOURCE_TYPE type = GetResType<T>();

	Res* res = FindRes<T>(_key).Get();
	if (nullptr != res)
	{
		return (T*)res;
	}
	
	res = new T;
	
	wstring filePath = PathManager::GetInst()->GetContentPath();
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

template <typename T>
inline void ResourceManager::AddRes(const wstring& _key, T* _res)
{
	RESOURCE_TYPE type = GetResType<T>();
	_res->SetKey(_key);
	m_mapRes[(UINT)type].insert(make_pair(_key, _res));
	m_bEvent = true;
}

template <typename T>
inline Ptr<T> ResourceManager::FindRes(const wstring& _key)
{
	RESOURCE_TYPE type = GetResType<T>();
	map<wstring, Res*>::iterator iter = m_mapRes[(UINT)type].find(_key);
	if (iter == m_mapRes[(UINT)type].end())
	{
		return nullptr;
	}

	return (T*)iter->second;
}
