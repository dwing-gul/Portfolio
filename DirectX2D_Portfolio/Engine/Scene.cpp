#include "pch.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

Scene::Scene() :
    m_type(SCENE_TYPE::PLAY_STAGE)
{
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i] = new Layer;
        m_arrLayer[i]->m_layerIndex = i;
        m_arrLayer[i]->m_ownerScene = this;
    }
}

Scene::~Scene()
{
    Safe_Delete_Arr(m_arrLayer);
}

void Scene::start()
{
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i]->start();
    }
}

void Scene::update()
{
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i]->update();
    }
}

void Scene::lateUpdate()
{
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i]->lateUpdate();
    }
}

void Scene::finalUpdate()
{
    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i]->finalUpdate();
    }
}

GameObject* Scene::FindObjByName(const wstring& _name)
{
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        const vector<GameObject*>& vectorObj = m_arrLayer[i]->GetParentObj();
        for (size_t i = 0; i < vectorObj.size(); ++i)
        {
            list<GameObject*> queue;
            queue.push_back(vectorObj[i]);

            while (!queue.empty())
            {
                GameObject* obj = queue.front();
                queue.pop_front();

                if (_name == obj->GetName())
                {
                    return obj;
                }

                const vector<GameObject*>& vectorChild = obj->GetChilds();
                for (size_t j = 0; j < vectorChild.size(); ++j)
                {
                    queue.push_back(vectorChild[j]);
                }
            }
        }
    }
    return nullptr;
}

const vector<GameObject*>& Scene::FindObjByType(OBJECT_TYPE _type)
{
    m_vectorTarget.clear();

    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        const vector<GameObject*>& vectorObj = m_arrLayer[i]->GetParentObj();
        for (size_t i = 0; i < vectorObj.size(); ++i)
        {
            if (_type == vectorObj[i]->GetObjectType())
                m_vectorTarget.push_back(vectorObj[i]);
        }
    }

    return m_vectorTarget;
}

vector<GameObject*>& Scene::FindObjByType_Ref(OBJECT_TYPE _type)
{
    m_vectorTarget.clear();

    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        const vector<GameObject*>& vectorObj = m_arrLayer[i]->GetParentObj();
        for (size_t i = 0; i < vectorObj.size(); ++i)
        {
            if (_type == vectorObj[i]->GetObjectType())
                m_vectorTarget.push_back(vectorObj[i]);
        }
    }

    return m_vectorTarget;
}

void Scene::AddObj(GameObject* _obj, int _layerIndex, bool _bWithChild)
{
    assert(!(_layerIndex < 0 || MAX_LAYER <= _layerIndex));

    m_arrLayer[_layerIndex]->AddObj(_obj, _bWithChild);
}

void Scene::DeregisterObj()
{
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i]->m_vectorObj.clear();
    }
}

void Scene::SaveToScene(FILE* _file)
{
    Entity::SaveToScene(_file);

    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i]->SaveToScene(_file);
    }

    SaveToFile(&m_type, _file);
}

void Scene::LoadFromScene(FILE* _file)
{
    Entity::LoadFromScene(_file);

    for (int i = 0; i < MAX_LAYER; ++i)
    {
        m_arrLayer[i]->LoadFromScene(_file);
    }

    LoadFromFile(&m_type, _file);
}
