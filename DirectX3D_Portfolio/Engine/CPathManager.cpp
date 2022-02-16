#include "pch.h"
#include "CPathManager.h"

CPathManager::CPathManager() :
	m_contentPath()
{
}

CPathManager::~CPathManager()
{
}

void CPathManager::init()
{
	wchar_t path[255] = {};
	GetCurrentDirectory(255, path);

	for (int i = (int)wcslen(path); i > 0; --i)
	{
		if (L'\\' == path[i])
		{
			path[i] = L'\0';
			break;
		}
	}

	m_contentPath = path;
	m_contentPath += L"\\bin\\content\\";
}

wstring CPathManager::GetRelativePath(const wstring& _path)
{
	if (-1 == _path.find(m_contentPath))
		return nullptr;

	wstring relativePath = _path.substr(m_contentPath.length(), _path.length());

	return relativePath;
}
