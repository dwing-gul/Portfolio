#include "stdafx.h"
#include "ResourceManager.h"
#include "PathManager.h"
#include "Texture.h"
#include "Sound.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	map<wstring, Texture*>::iterator iterTex = m_mapTexture.begin();
	for (; iterTex != m_mapTexture.end(); ++iterTex)
	{
		delete iterTex->second;
	}
	m_mapTexture.clear();

	map<wstring, Sound*>::iterator iterSound = m_mapSound.begin();
	for (; iterSound != m_mapSound.end(); ++iterSound)
	{
		delete iterSound->second;
	}
	m_mapSound.clear();
}

Texture* ResourceManager::FindTexture(const wstring& _key)
{
	map<wstring, Texture*>::iterator iter = m_mapTexture.find(_key);
	if (m_mapTexture.end() == iter)
	{
		return nullptr;
	}

	return iter->second;
}

Texture* ResourceManager::LoadTexture(const wstring& _key, const wstring& _relativePath)
{
	Res* tex = FindTexture(_key);

	if (nullptr != tex)
	{
		MessageBox(nullptr, L"텍스쳐 중복", L"텍스쳐 로딩 실패", MB_OK);
		return (Texture*)tex;
	}

	wstring filePath = CONTENT_PATH;
	filePath += _relativePath;

	tex = new Texture;
	HRESULT hr = tex->Load(filePath.c_str());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"텍스쳐 로딩 실패", L"리소스 로딩 실패", MB_OK);
		delete tex;
		return nullptr;
	}

	tex->SetKey(_key);
	tex->SetRelativePath(_relativePath);
	m_mapTexture.insert(make_pair(_key, (Texture*)tex));

	return (Texture*)tex;
}

Texture* ResourceManager::CreateTexture(const wstring& _key, UINT _width, UINT _height)
{
	Texture* tex = FindTexture(_key);

	if (nullptr != tex)
	{
		MessageBox(nullptr, L"텍스쳐 중복", L"텍스쳐 로딩 실패", MB_OK);
		return tex;
	}

	tex = new Texture;
	tex->Create(_width, _height);
	tex->SetKey(_key);
	m_mapTexture.insert(make_pair(_key, tex));

	return tex;
}

Sound* ResourceManager::LoadSound(const wstring& _key, const wstring& _relativePath)
{
	assert(nullptr == FindSound(_key));

	Sound* sound = new Sound;

	wstring strFilePath = PathManager::GetInst()->GetContentPath();
	strFilePath += _relativePath;
	HRESULT hr = sound->Load(strFilePath.c_str());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Sound 로딩 실패!!!", L"리소스 로딩 실패", MB_OK);
		delete sound;
		return nullptr;
	}

	sound->SetKey(_key);
	sound->SetRelativePath(_relativePath);

	// map 에 등록
	m_mapSound.insert(make_pair(_key, sound));

	return sound;
}

Sound* ResourceManager::FindSound(const wstring& _key)
{
	map<wstring, Sound*>::iterator iter = m_mapSound.find(_key);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

void ResourceManager::DeleteAllSound()
{
	map<wstring, Sound*>::iterator iter = m_mapSound.begin();

	for (; iter != m_mapSound.end(); ++iter)
	{
		iter->second->SoundRelease();
		delete iter->second;
	}

	m_mapSound.clear();
}
