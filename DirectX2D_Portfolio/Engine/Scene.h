#pragma once
#include "Entity.h"

class Layer;
class GameObject;

enum class SCENE_TYPE
{
    TOOL,
    LOADING_SCENE,
    PLAY_STAGE
};

class Scene :
    public Entity
{
private:
    Layer* m_arrLayer[MAX_LAYER];
    SCENE_TYPE  m_type;
    vector<GameObject*> m_vectorTarget; // 타입을 통해 찾은 오브젝트들을 임시로 담을 벡터

public:
    void start();
    void update();
    void lateUpdate();
    void finalUpdate();

    void SetSceneType(SCENE_TYPE _type) { m_type = _type; }
    SCENE_TYPE GetSceneType() { return m_type; }
    Layer* GetLayer(int _index)
    {
        assert(!(_index < 0 || MAX_LAYER <= _index));
        return m_arrLayer[_index];
    }

    GameObject* FindObjByName(const wstring& _name);
    const vector<GameObject*>& FindObjByType(OBJECT_TYPE _type);
    vector<GameObject*>& FindObjByType_Ref(OBJECT_TYPE _type);

private :
    void AddObj(GameObject* _obj, int _layerIndex, bool _bWithChild = true);
    void DeregisterObj();

public :
    void SaveToScene(FILE* _file);
    void LoadFromScene(FILE* _file);

    CLONE_DISABLE(Scene)

public :
    Scene();
    ~Scene();

    friend class SceneManager;
    friend class EventManager;
};

