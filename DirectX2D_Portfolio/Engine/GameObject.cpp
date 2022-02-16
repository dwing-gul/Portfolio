#include "pch.h"
#include "GameObject.h"
#include "MeshRender.h"
#include "Collider2D.h"
#include "Transform.h"
#include "Light2D.h"
#include "Particle.h"
#include "Camera.h"
#include "Animator2D.h"
#include "TileMap.h"
#include "Script.h"
#include "Camera.h"
#include "Prefab.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

GameObject::GameObject() :
    m_arrComponent{},
    m_type(OBJECT_TYPE::GENERAL),
    m_parent(nullptr),
    m_layerIndex(-1),
    m_bDead(false)
{
}

GameObject::GameObject(const GameObject& _origin) :
    Entity(_origin),
    m_arrComponent{},
    m_type(_origin.m_type),
    m_parent(nullptr),
    m_layerIndex(-1),
    m_bDead(false)
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr != _origin.m_arrComponent[i])
        {
            AddComponent(_origin.m_arrComponent[i]->Clone());
        }
    }

    for (size_t i = 0; i < _origin.m_vectorScript.size(); ++i)
    {
        AddComponent(_origin.m_vectorScript[i]->Clone());
    }

    for (size_t i = 0; i < _origin.m_vectorChild.size(); ++i)
    {
        AddChild(_origin.m_vectorChild[i]->Clone());
    }
}

GameObject::~GameObject()
{
    Safe_Delete_Arr(m_arrComponent);
    Safe_Delete_Vector(m_vectorScript);
    Safe_Delete_Vector(m_vectorChild);
}

void GameObject::start()
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComponent[i])
            continue;

        m_arrComponent[i]->start();
    }

    for (size_t i = 0; i < m_vectorScript.size(); ++i)
    {
        m_vectorScript[i]->start();
    }

    for (size_t i = 0; i < m_vectorChild.size(); ++i)
    {
        m_vectorChild[i]->start();
    }
}

void GameObject::update()
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComponent[i])
            continue;

        m_arrComponent[i]->update();
    }

    for (size_t i = 0; i < m_vectorScript.size(); ++i)
    {
       m_vectorScript[i]->update();
    }

    for (size_t i = 0; i < m_vectorChild.size(); ++i)
    {
        m_vectorChild[i]->update();
    }
}

void GameObject::lateUpdate()
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComponent[i])
            continue;

        m_arrComponent[i]->lateUpdate();
    }

    for (size_t i = 0; i < m_vectorScript.size(); ++i)
    {
        m_vectorScript[i]->lateUpdate();
    }

    for (size_t i = 0; i < m_vectorChild.size(); ++i)
    {
        m_vectorChild[i]->lateUpdate();
    }
}

void GameObject::finalUpdate()
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComponent[i])
            continue;

        m_arrComponent[i]->finalUpdate();
    }

    for (size_t i = 0; i < m_vectorChild.size(); ++i)
    {
        m_vectorChild[i]->finalUpdate();
    }

    RegisterLayer();
}

void GameObject::render()
{
    if (!m_bDead)
    {
        if (nullptr != GetMeshRender())
            GetMeshRender()->render();
        if (nullptr != GetTileMap())
            GetTileMap()->render();
        if (nullptr != GetParticle())
            GetParticle()->render();
        if (nullptr != GetCollider2D())
            GetCollider2D()->render();
        if (nullptr != GetLight2D())
            GetLight2D()->render();

        for (size_t i = 0; i < m_vectorScript.size(); ++i)
        {
            m_vectorScript[i]->render();
        }
    }
}

void GameObject::AddComponent(Component* _component)
{
    assert(COMPONENT_TYPE::SCRIPT == _component->GetType() || nullptr == m_arrComponent[(UINT)_component->GetType()]);

    COMPONENT_TYPE type = _component->GetType();

    if (COMPONENT_TYPE::MESH_RENDER == type || COMPONENT_TYPE::TILEMAP == type || COMPONENT_TYPE::PARTICLE == type)
    {
        for (UINT i = (UINT)COMPONENT_TYPE::MESH_RENDER; i < (UINT)COMPONENT_TYPE::PARTICLE + 1; ++i)
        {
            if (type == (COMPONENT_TYPE)i)
            {
                continue;
            }

            if (GetComponent((COMPONENT_TYPE)i))
                assert(nullptr);
        }
    }

    if (_component->GetType() == COMPONENT_TYPE::SCRIPT)
        m_vectorScript.push_back((Script*)_component);
    else
        m_arrComponent[(UINT)_component->GetType()] = _component;

    _component->m_obj = this;
}

void GameObject::SubComponent(COMPONENT_TYPE _type)
{
    if (nullptr == m_arrComponent[(UINT)_type])
        return;

    delete  m_arrComponent[(UINT)_type];
    m_arrComponent[(UINT)_type] = nullptr;
}

bool GameObject::IsAncestor(GameObject* _obj)
{
    GameObject* parent = m_parent;
    while (nullptr != parent)
    {
        if (_obj == parent)
        {
            return true;
        }

        parent = parent->GetParent();
    }

    return false;
}

void GameObject::SetDead()
{
    list<GameObject*> queue;
    queue.push_back(this);

    while (!queue.empty())
    {
        GameObject* obj = queue.front();
        queue.pop_front();

        obj->m_bDead = true;

        for (size_t i = 0; i < obj->m_vectorChild.size(); ++i)
        {
            queue.push_back(obj->m_vectorChild[i]);
        }
    }
}

void GameObject::RegisterAsPrefab(const wstring& _name)
{
    wstring name = _name;
    if (name.empty())
        name = GetName();

    Ptr<Prefab> prefab = ResourceManager::GetInst()->FindRes<Prefab>(name);

    if (nullptr == prefab.Get())
    {
        prefab = new Prefab(this->Clone());
        ResourceManager::GetInst()->AddRes<Prefab>(name, prefab.Get());
    }
}

void GameObject::RegisterLayer()
{
    Layer* layer = SceneManager::GetInst()->GetCurScene()->GetLayer(m_layerIndex);
    layer->RegisterObj(this);
}

void GameObject::AddChild(GameObject* _childObj)
{
    assert(false == IsAncestor(_childObj));
    
    if (nullptr == _childObj->GetParent() && -1 != _childObj->m_layerIndex)
    {
        Layer* layer = SceneManager::GetInst()->GetCurScene()->GetLayer(_childObj->m_layerIndex);
        layer->DeregisterObj(_childObj);
    }

    _childObj->DisconnectWithParent();

    m_vectorChild.push_back(_childObj);
    _childObj->m_parent = this;

    if (-1 == _childObj->m_layerIndex)
    {
        _childObj->m_layerIndex = m_layerIndex;
    }
}

void GameObject::DisconnectWithParent()
{
    if (nullptr == m_parent)
        return;

    vector<GameObject*>& vectorChild = m_parent->GetChild_Ref();
    vector<GameObject*>::iterator iter = vectorChild.begin();

    for (; iter != vectorChild.end(); ++iter)
    {
        if ((*iter) == this)
        {
            vectorChild.erase(iter);
            break;
        }
    }

    m_parent = nullptr;
}

void GameObject::SaveToScene(FILE* _file)
{
    Entity::SaveToScene(_file);

    if (GetParent())
    {
        SaveToFile(&m_layerIndex, _file);
    }

    UINT component = 0;
    for (; component < (UINT)COMPONENT_TYPE::END; ++component)
    {
        if (nullptr == m_arrComponent[component])
            continue;

        SaveToFile(&component, _file);
        m_arrComponent[component]->SaveToScene(_file);
    }
    SaveToFile(&component, _file);

    UINT scriptCount = (UINT)m_vectorScript.size();
    SaveToFile(&scriptCount, _file);

    for (size_t i = 0; i < m_vectorScript.size(); ++i)
    {
        SceneManager::GetInst()->m_saveScript(m_vectorScript[i], _file);
    }

    SaveToFile(&m_type, _file);

    UINT childCount = (UINT)m_vectorChild.size();
    SaveToFile(&childCount, _file);

    for (size_t i = 0; i < m_vectorChild.size(); ++i)
    {
        m_vectorChild[i]->SaveToScene(_file);
    }
}

void GameObject::LoadFromScene(FILE* _file, int _depth)
{
    Entity::LoadFromScene(_file);

    if (0 != _depth)
    {
        LoadFromFile(&m_layerIndex, _file);
    }

    UINT componentType = -1;
    while (true)
    {
        LoadFromFile(&componentType, _file);

        if ((UINT)COMPONENT_TYPE::END == componentType)
            break;

        Component* component = nullptr;
        switch ((COMPONENT_TYPE)componentType)
        {
        case COMPONENT_TYPE::TRANSFORM :
            component = new Transform;
            break;
        case COMPONENT_TYPE::BOUNDINGBOX :
            break;
        case COMPONENT_TYPE::COLLIDER2D :
            component = new Collider2D;
            break;
        case COMPONENT_TYPE::COLLIDER3D :
            break;
        case COMPONENT_TYPE::ANIMATOR2D :
            component = new Animator2D;
            break;
        case COMPONENT_TYPE::ANIMATOR3D :
            break;
        case COMPONENT_TYPE::LIGHT2D :
            component = new Light2D;
            break;
        case COMPONENT_TYPE::LIGHT3D :
            break;
        case COMPONENT_TYPE::CAMERA :
            component = new Camera;
            break;
        case COMPONENT_TYPE::LISTENER :
            break;
        case COMPONENT_TYPE::MESH_RENDER :
            component = new MeshRender;
            break;
        case COMPONENT_TYPE::TILEMAP :
            component = new TileMap;
            break;
        case COMPONENT_TYPE::PARTICLE :
            component = new Particle;
            break;
        }

        component->LoadFromScene(_file);
        AddComponent(component);
    }

    UINT scriptCount = 0;
    LoadFromFile(&scriptCount, _file);

    for (UINT i = 0; i < scriptCount; ++i)
    {
        Script* script = SceneManager::GetInst()->m_loadScript(_file);
        AddComponent(script);
    }

    LoadFromFile(&m_type, _file);

    ++_depth;

    UINT childCount = 0;
    LoadFromFile(&childCount, _file);

    for (UINT i = 0; i < childCount; ++i)
    {
        GameObject* childObject = new GameObject;
        childObject->LoadFromScene(_file, _depth);

        AddChild(childObject);
    }
}
