#pragma once
#include "Entity.h"
#include "GameObject.h"

class Component :
    public Entity
{
private : 
    GameObject*     m_obj;
    COMPONENT_TYPE  m_type;

public :
    COMPONENT_TYPE GetType() { return m_type; }

    virtual void start() {}
    virtual void update() {}
    virtual void lateUpdate() {}
    virtual void finalUpdate() {}

    GameObject* Object() { return m_obj; }

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Animator2D);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(Particle);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    virtual Component* Clone() = 0;

public :
    Component(COMPONENT_TYPE _type);
    Component(const Component& _origin);
    virtual ~Component();

    friend class GameObject;
};

