#pragma once

class CScene;
class CScript;

class CSceneSaveLoad
{
public :
	static HANDLE g_loadThread;
	static volatile int g_threadDead;

	static HANDLE g_loadStartEvent;
	static HANDLE g_loadExitEvent;

	static wstring g_relativePath;

	static void init();
	static void release();

	static void SaveScene(CScene* _scene, const wstring& _relativePath);
	static void LoadScene(const wstring& _relativePath);

	static void SaveScript(CScript* _script, FILE* _file);
	static CScript* LoadScript(FILE* _file);
};

