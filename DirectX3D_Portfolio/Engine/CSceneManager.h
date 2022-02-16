#pragma once

class CScene;
class CScript;
class CGameObject;

typedef CScript* (*LoadScript) (FILE*);
typedef void (*SaveScript) (CScript*, FILE*);

class CSceneManager :
	public CSingleton<CSceneManager>
{
	SINGLE(CSceneManager);
private:
	CScene* m_curScene;
	CScene* m_loadingScene;
	SCENE_MODE m_mode;

public:
	SaveScript m_saveScript;
	LoadScript m_loadScript;

public:
	void init();
	void progress();

	CScene* GetCurScene() { return m_curScene; }
	void ChangeScene(CScene* _scene);
	CGameObject* FindObjByName(const wstring& _name);
	vector<CGameObject*> FindObjByName_Vector(const wstring& _name);
	const vector<CGameObject*>& FindObjByType(OBJECT_TYPE _type, bool _bChild = false);
	const vector<CGameObject*>& FindObjByType(OBJECT_TYPE _type, int _layerIndex, bool _bChild = false);
	const vector<CGameObject*>& FindObjByLayerIndex(int _layerIndex, bool _bChild = false);
	vector<CGameObject*>& FindObjByType_Ref(OBJECT_TYPE _type, bool _bChild = false);
	SCENE_MODE GetSceneMode() { return m_mode; }
	CScene** GetNextScene() { return &m_nextScene; }
	volatile int& GetLoadSync() { return m_loadSync; }

	volatile int m_loadSync;
	CScene* m_nextScene;

	void SceneLoadStart();

private:
	void SetSceneMode(SCENE_MODE _mode);

	friend class CEventManager;
};


