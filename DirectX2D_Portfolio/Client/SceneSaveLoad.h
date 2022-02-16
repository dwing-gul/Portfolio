#pragma once

class Scene;
class Script;

class SceneSaveLoad
{
private :
	static HANDLE		g_loadThread;
	static volatile int g_threadDead;

	static HANDLE		g_loadStartEvent;
	static HANDLE		g_loadExitEvent;

	static wstring		g_relativePath;

public :
	static void init();
	static void release();

	static void SaveScene(Scene* _scene, const wstring& _relativePath);
	static void LoadScene(const wstring& _relativePath);

	static void SaveScript(Script* _script, FILE* _file);
	static Script* LoadScript(FILE* _file);

public :
	SceneSaveLoad();
	~SceneSaveLoad();

	friend UINT WINAPI Load_Thread(LPVOID _lParam);
};

