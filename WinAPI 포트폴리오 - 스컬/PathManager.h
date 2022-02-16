#pragma once
class PathManager
{
	SINGLE(PathManager)
private :
	wstring m_contentPath;

public :
	void init();
	wstring GetContentPath() { return m_contentPath; }
};

