#pragma once

class Scene;
class Script;
class GameObject;

typedef Script* (*LoadScript) (FILE*);
typedef void (*SaveScript) (Script*, FILE*);

class SceneManager :
	public Singleton<SceneManager>
{
	SINGLE(SceneManager)
private :
	Scene*		 m_curScene;
	Scene*		 m_loadingScene;
	Scene*		 m_nextScene;
	SCENE_MODE	 m_mode;
	volatile int m_loadSync;

public :
	SaveScript	 m_saveScript;
	LoadScript   m_loadScript;

public :
	void init();
	void progress();

	void SetLoadingScene(Scene* _scene);

	Scene* GetCurScene() { return m_curScene; }
	Scene* GetLoadingScene() { return m_loadingScene; }
	GameObject* FindObjByName(const wstring& _name);
	const vector<GameObject*>& FindObjByType(OBJECT_TYPE _type);
	vector<GameObject*>& FindObjByType_Ref(OBJECT_TYPE _type);
	SCENE_MODE GetSceneMode() { return m_mode; }
	Scene** GetNextScene() { return &m_nextScene; }
	volatile int& GetLoadSync() { return m_loadSync; }
	void SceneLoadStart();

private :
	void ChangeScene(Scene* _scene);
	void SetSceneMode(SCENE_MODE _mode);
	friend class EventManager;
	friend class SceneConverter;
};

