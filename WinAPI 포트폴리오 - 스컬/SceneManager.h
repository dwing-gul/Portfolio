#pragma once

class Scene;
class Obj;

class SceneManager
{
	SINGLE(SceneManager)
private :
	Scene* m_arrScene[(UINT)SCENE_TYPE::END];
	Scene* m_curScene;

	bool m_bGoPrev;

public :
	Scene* GetCurScene() { return m_curScene; }

	void init();
	void progress();
	void render(HDC _dc);

	void SetGoPrev(bool _prev) { m_bGoPrev = _prev; }

	Obj* GetObjectByName(const wstring& _name, GROUP_TYPE _group = GROUP_TYPE::END);
	void DeleteObjectByName(const wstring& _name, GROUP_TYPE _group = GROUP_TYPE::END);
	Scene* GetNextScene(SCENE_TYPE _scene) 
	{
		if(SCENE_TYPE((int)_scene + 1) != SCENE_TYPE::END)
			return m_arrScene[(int)_scene + 1]; 

		return nullptr;
	}
	Scene* GetPrevScene(SCENE_TYPE _scene)
	{
		if (SCENE_TYPE((int)_scene - 1) != SCENE_TYPE::TOOL)
			return m_arrScene[(int)_scene - 1];

		return nullptr;
	}

	bool IsGoPrev() { return m_bGoPrev; }

	void ChangeScene(SCENE_TYPE _eType);

public :
	friend class EventManager;
};

