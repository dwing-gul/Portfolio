#pragma once
class Res
{
private:
	wstring m_key;
	wstring m_relativePath;

	void SetKey(const wstring& _key) { m_key = _key; }
	void SetRelativePath(const wstring& _relativePath) { m_relativePath = _relativePath; }

	virtual int Load(const wchar_t* _relativePath) = 0;

public :
	const wstring& GetKey() { return m_key; }
	const wstring& GetRelativePath() { return m_relativePath; }

public :
	Res();
	virtual ~Res();

	friend class ResourceManager;
};

