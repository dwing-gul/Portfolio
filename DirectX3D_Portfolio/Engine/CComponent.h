#pragma once
#include "CEntity.h"
#include "CGameObject.h"

class CComponent :
    public CEntity
{
private:
    CGameObject* m_obj;
    COMPONENT_TYPE m_type;

public:
    COMPONENT_TYPE GetType() { return m_type; }

    virtual void start() {}
    virtual void update() {}
    virtual void lateUpdate() {}
    virtual void finalUpdate() {}

    CGameObject* Object() { return m_obj; }

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Collider3D);
    GET_OTHER_COMPONENT(Animator2D);
    GET_OTHER_COMPONENT(Animator3D);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(Light3D);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(ParticleSystem);
    GET_OTHER_COMPONENT(NaviMesh);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    virtual CComponent* Clone() = 0;

public:
    CComponent(COMPONENT_TYPE _type);
    CComponent(const CComponent& _origin);
    virtual ~CComponent();

    friend class CGameObject;
};


