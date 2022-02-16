#include "pch.h"
#include "CSceneSaveLoad.h"
#include <Engine/CPathManager.h>
#include <Engine/CScene.h>
#include <Engine/CScript.h>
#include <Script/CScriptManager.h>

HANDLE CSceneSaveLoad::g_loadThread = nullptr;
volatile int CSceneSaveLoad::g_threadDead = 0;

HANDLE CSceneSaveLoad::g_loadStartEvent = nullptr;
HANDLE CSceneSaveLoad::g_loadExitEvent = nullptr;

wstring CSceneSaveLoad::g_relativePath;

UINT WINAPI Load_Thread(LPVOID _lParam)
{
	CScene*& nextScene = *((CScene**)_lParam);

	while (true)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(CSceneSaveLoad::g_loadStartEvent, 0))
		{
			continue;
		}

		if (CSceneSaveLoad::g_threadDead)
		{
			break;
		}

		ResetEvent(CSceneSaveLoad::g_loadStartEvent);

		CScene* scene = new CScene;

		wstring filePath = CPathManager::GetInst()->GetContentPath();
		filePath += CSceneSaveLoad::g_relativePath;

		FILE* file = nullptr;
		_wfopen_s(&file, filePath.c_str(), L"rb");

		assert(file);

		scene->LoadFromScene(file);

		fclose(file);

		nextScene = scene;
		CSceneManager::GetInst()->GetLoadSync() = 1;
	}

	SetEvent(CSceneSaveLoad::g_loadExitEvent);

	_endthreadex(0);
	return 0;
}

void CSceneSaveLoad::init()
{
	g_loadThread = (HANDLE)_beginthreadex(nullptr, 0, Load_Thread, CSceneManager::GetInst()->GetNextScene(), 0, nullptr);

	g_loadStartEvent = CreateEvent(nullptr, true, false, L"Thread Continue Event");
	g_loadExitEvent = CreateEvent(nullptr, true, false, L"Thread Exit Event");
	CSceneManager::GetInst()->m_saveScript = &CSceneSaveLoad::SaveScript;
	CSceneManager::GetInst()->m_loadScript = &CSceneSaveLoad::LoadScript;
}

void CSceneSaveLoad::release()
{
	g_threadDead = 1;
	SetEvent(g_loadStartEvent);
	CloseHandle(g_loadThread);

	while (WAIT_OBJECT_0 != WaitForSingleObject(g_loadExitEvent, 0)) {}
}

void CSceneSaveLoad::SaveScene(CScene* _scene, const wstring& _relativePath)
{
	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _relativePath;

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");

	assert(file);

	_scene->SaveToScene(file);

	fclose(file);
}

void CSceneSaveLoad::LoadScene(const wstring& _relativePath)
{
	g_relativePath = _relativePath;

	CSceneManager::GetInst()->SceneLoadStart();

	SetEvent(g_loadStartEvent);
}

void CSceneSaveLoad::SaveScript(CScript* _script, FILE* _file)
{
	wstring scriptName = CScriptManager::GetScriptName(_script);
	SaveWStringToFile(scriptName, _file);

	_script->SaveToScene(_file);
}

CScript* CSceneSaveLoad::LoadScript(FILE* _file)
{
	wstring scriptName;
	LoadWStringFromFile(scriptName, _file);

	CScript* script = CScriptManager::GetScript(scriptName);
	script->LoadFromScene(_file);

	return script;
}
