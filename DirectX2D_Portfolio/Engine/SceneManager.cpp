#include "pch.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "Scene.h"

SceneManager::SceneManager() :
    m_curScene(nullptr),
    m_loadingScene(nullptr),
    m_nextScene(nullptr),
    m_mode(SCENE_MODE::STOP),
    m_loadSync(0),
    m_saveScript(nullptr),
    m_loadScript(nullptr)
{
}

SceneManager::~SceneManager()
{
    Safe_Delete(m_curScene);
    Safe_Delete(m_loadingScene);
}

void SceneManager::init()
{
    if (nullptr == m_loadingScene)
    {
        m_loadingScene = new Scene;
        m_loadingScene->SetSceneType(SCENE_TYPE::LOADING_SCENE);
    }
}

void SceneManager::progress()
{
    if (m_loadSync)
    {
        m_loadSync = 0;
        ::ChangeScene(m_nextScene);
    }

    if (nullptr == m_curScene)
        return;

    m_curScene->DeregisterObj();

    if (SCENE_MODE::PLAY == m_mode)
    {
        m_curScene->update();
        m_curScene->lateUpdate();
    }

    RenderManager::GetInst()->DeregisterCamera();

    m_curScene->finalUpdate();

    if (SCENE_MODE::PLAY == m_mode)
    {
        CollisionManager::GetInst()->update();
    }
}

void SceneManager::SetLoadingScene(Scene* _scene)
{
    Safe_Delete(m_loadingScene); 
    m_loadingScene = _scene;
}

GameObject* SceneManager::FindObjByName(const wstring& _name)
{
    return m_curScene->FindObjByName(_name);
}

const vector<GameObject*>& SceneManager::FindObjByType(OBJECT_TYPE _type)
{
    return m_curScene->FindObjByType(_type);
}

vector<GameObject*>& SceneManager::FindObjByType_Ref(OBJECT_TYPE _type)
{
    return m_curScene->FindObjByType_Ref(_type);
}

void SceneManager::SceneLoadStart()
{
    m_loadSync = 0;
    
    Safe_Delete(m_curScene);

    m_curScene = m_loadingScene;
}

void SceneManager::ChangeScene(Scene* _scene)
{
    if (m_curScene && m_curScene != m_loadingScene)
    {
        delete m_curScene;
    }

    m_curScene = _scene;
}

void SceneManager::SetSceneMode(SCENE_MODE _mode)
{
    if (SCENE_MODE::PLAY == _mode && SCENE_MODE::STOP == m_mode)
    {
        m_curScene->start();
    }

    m_mode = _mode;
}
