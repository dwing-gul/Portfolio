#pragma once

class Texture;
class Sound;

class ResourceManager
{
	SINGLE(ResourceManager)
private :
	map<wstring, Texture*> m_mapTexture;
	map<wstring, Sound*> m_mapSound;

public :
	Texture* FindTexture(const wstring& _key);
	Texture* LoadTexture(const wstring& _key, const wstring& _relativePath);
	Texture* CreateTexture(const wstring& _key, UINT _width, UINT _height);

	Sound* LoadSound(const wstring& _key, const wstring& _relativePath);
	Sound* FindSound(const wstring& _key);
	void DeleteAllSound();
};

