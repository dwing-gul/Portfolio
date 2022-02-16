#include "stdafx.h"
#include "PathManager.h"

PathManager::PathManager() :
	m_contentPath()
{
}

PathManager::~PathManager()
{
}

void PathManager::init()
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
