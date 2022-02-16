#pragma once

class CPathManager :
	public CSingleton<CPathManager>
{
	SINGLE(CPathManager);
private :
	wstring m_contentPath;

public :
	void init();
	const wchar_t* GetContentPath() { return m_contentPath.c_str(); }
	wstring GetRelativePath(const wstring& _path);
};

