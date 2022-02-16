#include "pch.h"
#include "CSceneManager.h"
#include "CCollisionManager.h"
#include "CRenderManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CScene.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"

#include "Ptr.h"
#include "CMesh.h"

CSceneManager::CSceneManager() :
	m_curScene(nullptr),
	m_loadingScene(nullptr),
	m_nextScene(nullptr),
	m_loadSync(0),
	m_mode(SCENE_MODE::STOP),
	m_loadScript(nullptr),
	m_saveScript(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	Safe_Delete_Ptr(m_curScene);
	Safe_Delete_Ptr(m_loadingScene);
}

void CSceneManager::init()
{
	m_loadingScene = new CScene;
}

void CSceneManager::progress()
{
	if (m_loadSync)
	{
		m_loadSync = 0;
		::ChangeScene(m_nextScene);
	}

	if (nullptr == m_curScene)
		return;

	if (KEY_TAP(KEY::B) && SCENE_MODE::STOP == m_mode)
	{
		m_curScene->start();
	}

	if (SCENE_MODE::PLAY == m_mode)
	{
		m_curScene->update();
		m_curScene->lateUpdate();
	}

	m_curScene->DeregisterObject();

	CRenderManager::GetInst()->DeregisterCamera();

	CCamera* mainCam = CRenderManager::GetInst()->GetMainCam();

	m_curScene->finalUpdate();

	if (SCENE_MODE::PLAY == m_mode)
	{
		CCollisionManager::GetInst()->update();
	}
}

void CSceneManager::ChangeScene(CScene* _scene)
{
	if (m_curScene && m_curScene != m_loadingScene)
	{
		delete m_curScene;
	}

	m_curScene = _scene;
}

CGameObject* CSceneManager::FindObjByName(const wstring& _name)
{
	return m_curScene->FindObjByName(_name);
}

vector<CGameObject*> CSceneManager::FindObjByName_Vector(const wstring& _name)
{
	vector<CGameObject*> vecObject = m_curScene->FindObjByName_Vector(_name);
	return vecObject;
}

const vector<CGameObject*>& CSceneManager::FindObjByType(OBJECT_TYPE _type, bool _bChild)
{
	return m_curScene->FindObjByType(_type, _bChild);
}

const vector<CGameObject*>& CSceneManager::FindObjByType(OBJECT_TYPE _type, int _layerIndex, bool _bChild)
{
	return m_curScene->FindObjByType(_type, _layerIndex, _bChild);
}

const vector<CGameObject*>& CSceneManager::FindObjByLayerIndex(int _layerIndex, bool _bChild)
{
	return m_curScene->FindObjByLayerIndex(_layerIndex, _bChild);
}

vector<CGameObject*>& CSceneManager::FindObjByType_Ref(OBJECT_TYPE _type, bool _bChild)
{
	vector<CGameObject*> empty;

	if (nullptr != m_curScene)
		return m_curScene->FindObjByType_Ref(_type, _bChild);
	else
		return empty;
}

void CSceneManager::SetSceneMode(SCENE_MODE _mode)
{
	if (SCENE_MODE::PLAY == _mode && SCENE_MODE::STOP == m_mode)
	{
		m_curScene->start();
	}

	m_mode = _mode;
}

void CSceneManager::SceneLoadStart()
{
	m_loadSync = 0;

	if (m_curScene)
	{
		delete m_curScene;
	}

	m_curScene = m_loadingScene;
}