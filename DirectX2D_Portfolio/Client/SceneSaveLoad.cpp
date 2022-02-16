#include "pch.h"
#include "SceneSaveLoad.h"
#include <Script/ScriptManager.h>
#include <Engine/PathManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Scene.h>
#include <Engine/Script.h>

HANDLE SceneSaveLoad::g_loadThread = nullptr;
volatile int SceneSaveLoad::g_threadDead = 0;

HANDLE SceneSaveLoad::g_loadStartEvent = nullptr;
HANDLE SceneSaveLoad::g_loadExitEvent = nullptr;

wstring SceneSaveLoad::g_relativePath;

UINT WINAPI Load_Thread(LPVOID _lParam);

SceneSaveLoad::SceneSaveLoad()
{
}

SceneSaveLoad::~SceneSaveLoad()
{
}

void SceneSaveLoad::init()
{
    g_loadThread = (HANDLE)_beginthreadex(nullptr, 0, Load_Thread, SceneManager::GetInst()->GetNextScene(), 0, nullptr);
    g_loadStartEvent = CreateEvent(nullptr, true, false, L"Thread Continue Event");
    g_loadExitEvent = CreateEvent(nullptr, true, false, L"Thread Exit Event");
    SceneManager::GetInst()->m_saveScript = &SceneSaveLoad::SaveScript;
    SceneManager::GetInst()->m_loadScript = &SceneSaveLoad::LoadScript;
}

void SceneSaveLoad::release()
{
    g_threadDead = 1;
    SetEvent(g_loadStartEvent);
    CloseHandle(g_loadThread);
    while (WAIT_OBJECT_0 != WaitForSingleObject(g_loadStartEvent, 0)) {}
}

void SceneSaveLoad::SaveScene(Scene* _scene, const wstring& _relativePath)
{
    wstring filePath = PathManager::GetInst()->GetContentPath();
    filePath += _relativePath;
    const wchar_t* check = wcsstr(_relativePath.c_str(), L".scene");

    if (check == NULL)
    {
        filePath += L".scene";
    }

    FILE* file = nullptr;
    _wfopen_s(&file, filePath.c_str(), L"wb");

    assert(file);

    _scene->SaveToScene(file);

    fclose(file);
}

void SceneSaveLoad::LoadScene(const wstring& _relativePath)
{
    g_relativePath = _relativePath;

    SceneManager::GetInst()->SceneLoadStart();
    SetEvent(g_loadStartEvent);
}

void SceneSaveLoad::SaveScript(Script* _script, FILE* _file)
{
    wstring scriptName = ScriptManager::GetScriptName(_script);
    SaveWStringToFile(scriptName, _file);

    _script->SaveToScene(_file);
}

Script* SceneSaveLoad::LoadScript(FILE* _file)
{
    wstring scriptName;
    LoadWStringFromFile(scriptName, _file);

    Script* script = ScriptManager::GetScript(scriptName);
    script->LoadFromScene(_file);

    return script;
}

UINT WINAPI Load_Thread(LPVOID _lParam)
{
    Scene*& nextScene = *((Scene**)_lParam);

    while (true)
    {
        if (WAIT_OBJECT_0 != WaitForSingleObject(SceneSaveLoad::g_loadStartEvent, 0))
        {
            continue;
        }

        if (SceneSaveLoad::g_threadDead)
        {
            break;
        }

        ResetEvent(SceneSaveLoad::g_loadStartEvent);

        Scene* scene = new Scene;

        wstring filePath = PathManager::GetInst()->GetContentPath();
        filePath += SceneSaveLoad::g_relativePath;

        FILE* file = nullptr;
        _wfopen_s(&file, filePath.c_str(), L"rb");

        assert(file);

        scene->LoadFromScene(file);

        fclose(file);

        nextScene = scene;
        SceneManager::GetInst()->GetLoadSync() = 1;
    }

    SetEvent(SceneSaveLoad::g_loadExitEvent);

    _endthreadex(0);
    return 0;
}