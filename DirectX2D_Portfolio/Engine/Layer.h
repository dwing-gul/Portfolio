#pragma once
#include "Entity.h"

class GameObject;
class Scene;

class Layer :
    public Entity
{
private :
    Scene*              m_ownerScene;
    vector<GameObject*> m_vectorParentObj;
    vector<GameObject*> m_vectorObj;
    int                 m_layerIndex;

public :
    void start();
    void update();
    void lateUpdate();
    void finalUpdate();

    void RegisterObj(GameObject* _obj) { m_vectorObj.push_back(_obj); }
    void DeregisterObj(GameObject* _obj);

    const vector<GameObject*>& GetParentObj() { return m_vectorParentObj; }
    const vector<GameObject*>& GetAllObj() { return m_vectorObj; }

private :
    void AddObj(GameObject* _obj, bool _bWithChild);
    void ResignObj(GameObject* _obj);
    void RegisterAsParent(GameObject* _obj) { m_vectorParentObj.push_back(_obj); }

public :
    void SaveToScene(FILE* _file);
    void LoadFromScene(FILE* _file);

    CLONE_DISABLE(Layer);

public :
    Layer();
    ~Layer();

    friend class Scene;
    friend class EventManager;
};

