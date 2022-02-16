#pragma once
#include "Entity.h"

class Component;
class Transform;
class MeshRender;
class Light2D;
class Camera;
class Collider2D;
class Animator2D;
class TileMap;
class Particle;
class Script;

class GameObject :
    public Entity
{
private :
    Component*          m_arrComponent[(UINT)COMPONENT_TYPE::END];
    vector<Script*>     m_vectorScript;
    GameObject*         m_parent;
    vector<GameObject*> m_vectorChild;
    OBJECT_TYPE         m_type;

    int                 m_layerIndex;
    bool                m_bDead;

public :
    void start();
    void update();
    void lateUpdate();
    void finalUpdate();
    void render();

    void SetObjectType(OBJECT_TYPE _type) { m_type = _type; }
    void AddComponent(Component* _component);
    void SubComponent(COMPONENT_TYPE _type); // 스크립트를 제외한 컴포넌트 삭제
    OBJECT_TYPE GetObjectType() { return m_type; }
    Component* GetComponent(COMPONENT_TYPE _type) { return m_arrComponent[(UINT)_type]; }
    GameObject* GetParent() { return m_parent; }
    int GetLayerIndex() { return m_layerIndex; }
    const vector<GameObject*>& GetChilds() const { return m_vectorChild; }
    vector<GameObject*>& GetChild_Ref() { return m_vectorChild; }
    bool IsAncestor(GameObject* _obj);
    bool IsDead() { return m_bDead; }
    const vector<Script*>& GetScripts() { return m_vectorScript; }
    vector<Script*>& GetScripts_Ref() { return m_vectorScript; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESH_RENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(Particle, PARTICLE);

    void RegisterAsPrefab(const wstring& _name = L"");

private :
    void SetDead();
    void RegisterLayer();
    void AddChild(GameObject* _childObj);
    void DisconnectWithParent();

public :
    void SaveToScene(FILE* _file);
    void LoadFromScene(FILE* _file, int _depth);

    CLONE(GameObject)

public :
    GameObject();
    GameObject(const GameObject& _origin);
    ~GameObject();

    friend class EventManager;
    friend class Layer;
};

